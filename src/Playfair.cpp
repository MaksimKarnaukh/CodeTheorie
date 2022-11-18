//
// Created by Maksim Karnaukh on 20/10/2022.
//

#include "Playfair.h"
#include <cmath>

Playfair::Playfair(const string &filename) : AlgorithmDecryption(filename) {

}

std::string Playfair::Solve() {


//    vector<string> parent_key; // representation of 5x5 matrix. Each entry is a row consisting of 5 chars.
    string key = "ABCDEFGHIKLMNOPQRSTUVWXYZ";
//    for (auto i = 0; i < alfabet_string.size(); i+=5) {
//        parent_key.push_back(alfabet_string.substr(i, 5));
//    }

    string ciphertext = this->getCipherText();
    string plaintext = decipher(key, ciphertext);

    std::vector<float> freq = getAlphabetFrequencies(plaintext);
    float best_fitness = compareFrequencies(freq, LETTER_FREQUENCY_EN);

    for (float TEMP = 2000; TEMP >= 0; TEMP = TEMP - 0.1f) {
        for (int count = 50000; count > 0; count--) {
            string temp_key = modifyKey(key);
            string temp_plaintext = decipher(temp_key, ciphertext);
            std::vector<float> temp_freq = getAlphabetFrequencies(plaintext);
            float temp_fitness = compareFrequencies(temp_freq, LETTER_FREQUENCY_EN);
            float dF = (-temp_fitness) - (-best_fitness);
            if (dF >= 0) {
                key = temp_key;
                best_fitness = temp_fitness;
            }
            else {
                if (TEMP > 0) {
                    float prob = exp((float)(dF)/TEMP);
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

    string temp_key;
    int r = ((double) rand() / (RAND_MAX))*100;
    if (r >= 98) { // Swap random rows (2%)

    }
    else if (r >= 96) { // Swap columns (2%)

    }
    else if (r >= 94) { // Flip all rows (2%)

    }
    else if (r >= 92) { // Flip all columns (2%)

    }
    else if (r >= 90) { // Reverse the whole key (2%)

    }
    else { // Swap single letters (90%)

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
