//
// Created by Maksim Karnaukh on 20/10/2022.
//

#include "Playfair.h"

Playfair::Playfair(const string &filename) : AlgorithmDecryption(filename) {

}

std::string Playfair::Solve() {

    std::random_device rd;
    random_engine = mt19937(rd()); // seeding the random generator
    int fitness_modifier = 600; // we need this so that our probability formula with dF works better (600 turned out be a decent value).

    string key = "ABCDEFGHIKLMNOPQRSTUVWXYZ";

    string ciphertext = this->getCipherText();

//    key = "PVNBHQYECKRZUDLMXISFOWATG";
//    key = "ANEUITBCDSGHKLFOPQRMWXYZV";
//    key = "IANEUSTBCDFGHKLMOPQRVWXYZ";

    string plaintext = decipher(key, ciphertext);

    std::vector<double> freq;
    std::vector<double> LETTER_FREQUENCY_EN_MODIFIED = LETTER_FREQUENCY_EN;
    LETTER_FREQUENCY_EN_MODIFIED[8] = LETTER_FREQUENCY_EN_MODIFIED[8] + LETTER_FREQUENCY_EN_MODIFIED[9];
    LETTER_FREQUENCY_EN_MODIFIED[9] = 0; // playfair doesn't use the letter 'J', so we 'remove' that frequency.
    normalize(LETTER_FREQUENCY_EN_MODIFIED);
    getAlphabetFrequencies(plaintext,freq);
    double best_fitness = (1-compareFrequencies(freq, LETTER_FREQUENCY_EN_MODIFIED))*fitness_modifier;

    cout << endl << "plain: " << plaintext << endl << "fitness: " << best_fitness/fitness_modifier << endl;

    string temp_key, temp_plaintext;
    double temp_fitness, dF, prob;

    std::fstream ofs; // file to write our output to (deciphered text, key and fitness)
    ofs.open("playfairTest.txt", std::ofstream::out | std::ofstream::trunc);
    if (ofs.is_open()) {
        std::cout << "Opened playfair text file";
    } else std::cout << "Unable to open file";
    ofs << plaintext << std::endl << "^^ " << key << ", " << best_fitness/fitness_modifier << std::endl;

    double max_fitness = 0;
    string bestfittext;

    for (double TEMP = 300; TEMP > 0; TEMP = TEMP - 0.1) { // we need to choose a good value for TEMP (temperature) ourselves.
        for (int count = 50000; count > 0; count--) { // we need to choose a good value for count ourselves.
            temp_key = modifyKey(key);
            temp_plaintext = decipher(temp_key, ciphertext);
            std::vector<double> temp_freq;
            getAlphabetFrequencies(temp_plaintext,temp_freq);
            temp_fitness = (1-compareFrequencies(temp_freq, LETTER_FREQUENCY_EN_MODIFIED))*fitness_modifier;
            dF = temp_fitness - best_fitness;

            if (dF >= 0) {
                key = temp_key;
                best_fitness = temp_fitness;
                if (temp_fitness > max_fitness) {
                    max_fitness = temp_fitness;
                    bestfittext = temp_plaintext;
                    ofs << temp_plaintext << std::endl << "^ " << temp_key << ", " << temp_fitness/fitness_modifier << std::endl;
                }
            }
            else {
                prob = exp(((double)(dF)/TEMP));
                if (prob > 1.0*rand()/RAND_MAX) { // 1.0*rand()/RAND_MAX returns a random float between 0 and 1.
                    best_fitness = temp_fitness;
                    key = temp_key;

                    if (temp_fitness > max_fitness) {
                        max_fitness = temp_fitness;
                        bestfittext = temp_plaintext;
                        ofs << temp_plaintext << std::endl << "^^ " << temp_key << ", " << temp_fitness/fitness_modifier << std::endl; // writing to file
                    }
                }
            }
        }
    }
    cout << "Best fit text: " << bestfittext << endl;
    return key;
}

int Playfair::mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}

string Playfair::modifyKey(const string& key) {

    string temp_key = key;

    uniform_int_distribution<int> uint_dist(0, 100);
    int r = uint_dist(random_engine); // r is an integer between 0 and 100.

    if (r < 90) { // Swap single letters (90%)
        temp_key = swapTwoLetters(temp_key);
    }
    else if (r < 92) { // Reverse the whole key (2%)
        reverse(temp_key.begin(), temp_key.end());
    }
    else if (r < 94) { // Flip all rows (2%)
        temp_key = flipAllRows(temp_key);
    }
    else if (r < 96) { // Flip all columns (2%)
        temp_key = flipAllColumns(temp_key);
    }
    else if (r < 98) { // Swap columns (2%)
        temp_key = swapTwoColumns(temp_key);
    }
    else { // Swap random rows (2%)
        temp_key = swapTwoRows(temp_key);
    }
    return temp_key;

}

string Playfair::decipher(const string& key, const string& ciphertext) {
    string plaintext;
    for (int i = 0, ciphertext_size = ciphertext.size(); i < ciphertext_size; i += 2) {

        char first_letter = ciphertext[i];
        char second_letter = ciphertext[i + 1];
        int first_letter_index = key.find(first_letter);
        int second_letter_index = key.find(second_letter);

        if (first_letter_index / 5 == second_letter_index / 5) { // same row
            first_letter = key[(mod(first_letter_index - 1, 5)) + (first_letter_index / 5) * 5];
            second_letter = key[(mod(second_letter_index - 1, 5)) + (second_letter_index / 5) * 5];
        } else if (first_letter_index % 5 == second_letter_index % 5) { // same column
            first_letter = key[((mod(first_letter_index / 5 - 1, 5)) * 5 + first_letter_index % 5)];
            second_letter = key[((mod(second_letter_index / 5 - 1, 5)) * 5 + second_letter_index % 5)];
        } else { // different row and column
            short isFirstLetterFirst = -1;
            if (first_letter_index % 5 < second_letter_index % 5) {
                isFirstLetterFirst = 1;
            }
            first_letter = key[((first_letter_index + isFirstLetterFirst * abs((first_letter_index % 5) - (second_letter_index % 5))) %5) + (first_letter_index / 5) * 5];
            second_letter = key[((second_letter_index - isFirstLetterFirst * abs((first_letter_index % 5) - (second_letter_index %5))) % 5) +(second_letter_index / 5) * 5];
        }

        if (plaintext[plaintext.size() - 2] == first_letter && plaintext[plaintext.size() - 1] == 'X') { // remove the extra placed letter
            plaintext.pop_back();
        }
        plaintext.push_back(first_letter);
        plaintext.push_back(second_letter);
    }
    return plaintext;
}

string Playfair::swapTwoLetters(const string &key) {
    string temp_key = key;
    uniform_int_distribution<int> temp_uint_dist(0, 24);
    int first_index = temp_uint_dist(random_engine);
    int second_index = temp_uint_dist(random_engine);

    swap(temp_key[first_index], temp_key[second_index]);
    return temp_key;
}

string Playfair::swapTwoRows(const string &key) {
    string temp_key = key;
    uniform_int_distribution<int> temp_uint_dist(0, 4);
    int first_index = temp_uint_dist(random_engine);
    int second_index = temp_uint_dist(random_engine);

    for (int col_index = 0; col_index < 5; col_index++) {
        char temp_char = temp_key[first_index*5+col_index];
        temp_key[first_index*5+col_index] = temp_key[second_index*5+col_index];
        temp_key[second_index*5+col_index] = temp_char;
    }
    return temp_key;
}

string Playfair::swapTwoColumns(const string &key) {
    string temp_key = key;
    uniform_int_distribution<int> temp_uint_dist(0, 4);
    int first_index = temp_uint_dist(random_engine);
    int second_index = temp_uint_dist(random_engine);

    for (int row_index = 0; row_index < 5; row_index++) {
        char temp_char = temp_key[row_index*5+first_index];
        temp_key[row_index*5+first_index] = temp_key[row_index*5+second_index];
        temp_key[row_index*5+second_index] = temp_char;
    }
    return temp_key;
}

string Playfair::flipAllRows(const string &key) {
    string temp_key = key;
    for (int row_index = 0; row_index < 2; row_index++) {
        for (int col_index = 0; col_index < 5; col_index++) {
            swap(temp_key[(5*row_index)+col_index], temp_key[(5*(4-row_index))+col_index]);
        }
    }
    return temp_key;
}

string Playfair::flipAllColumns(const string &key) {
    string temp_key = key;
    for (int col_index = 0; col_index < 2; col_index++) {
        for (int row_index = 0; row_index < 5; row_index++) {
            swap(temp_key[(5*row_index)+col_index], temp_key[(5*(row_index))+(4-col_index)]);
        }
    }
    return temp_key;
}
