//
// Created by Maksim Karnaukh on 20/10/2022.
//

#include "Playfair.h"

Playfair::Playfair(const string &filename) : AlgorithmDecryption(filename) {

}

std::string Playfair::Solve() {

    std::random_device rd;
    random_engine = mt19937(rd()); // seeding the generator
//    vector<string> parent_key; // representation of 5x5 matrix. Each entry is a row consisting of 5 chars.
    string key = "ABCDEFGHIKLMNOPQRSTUVWXYZ";
//    for (auto i = 0; i < alfabet_string.size(); i+=5) {
//        parent_key.push_back(alfabet_string.substr(i, 5));
//    }

    string ciphertext = this->getCipherText();
    string plaintext = decipher(key, ciphertext);

    std::vector<double> freq;
    getAlphabetFrequencies(plaintext,freq);
    double best_fitness = compareFrequencies(freq, LETTER_FREQUENCY_EN);

    for (double TEMP = 1000; TEMP >= 0; TEMP = TEMP - 0.1f) { // 2000
        for (int count = 10000; count > 0; count--) { // 50000
            string temp_key = modifyKey(key);
            string temp_plaintext = decipher(temp_key, ciphertext);
            std::vector<double> temp_freq;
            getAlphabetFrequencies(temp_plaintext,freq);
            double temp_fitness = compareFrequencies(temp_freq, LETTER_FREQUENCY_EN);
            double dF = (-temp_fitness) - (-best_fitness);
            if (count % 1000 == 0) {
                cout << "----- TEMP : " << std::setprecision(10) << TEMP << " , COUNT : " << count << endl;
                cout << "temp_key : " << temp_key << endl;
                cout << "temp_fitness : " << std::setprecision(10) << temp_fitness << endl;
                cout << "dF : " << std::setprecision(10) << dF << endl;
            }
            if (dF >= 0) {
                key = temp_key;
                best_fitness = temp_fitness;
            }
            else {
                if (TEMP > 0) {
                    double prob = exp((double)(dF)/TEMP);
                    if (prob > 1.0*rand()/RAND_MAX) {
                        best_fitness = temp_fitness;
                        key = temp_key;
                    }
                }
            }
        }
    }

    return key;

}

int Playfair::mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}

bool Playfair::hasBetterFitness(const map<int, set<basic_string<char>>>& freq1, const map<int, set<basic_string<char>>>& freq2) const {
//    if they have the same amount of maximum frequencies
    if (freq1.rbegin()->first == freq2.rbegin()->first) {
//        the one with more entries is better
        return freq1.rbegin()->second.size() > freq2.rbegin()->second.size();
    }
//    the one with the largers maximum frequencies is better
    return freq1.rbegin()->first > freq2.rbegin()->first;
}

int Playfair::getFitness(const map<int, set<basic_string<char>>>& freq) const {

    return freq.rbegin()->first;
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
    for (auto i = 0; i < ciphertext.size(); i += 2) {

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

        if (plaintext[plaintext.size() - 2] == first_letter && plaintext[plaintext.size() - 1] == 'X') {
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
