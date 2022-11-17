//
// Created by Maksim Karnaukh on 20/10/2022.
//

#include "Playfair.h"
#include <cmath>

Playfair::Playfair(const string &filename) : AlgorithmDecryption(filename) {

}

std::string Playfair::Solve() {

    /*
    1. Generate a random key, called the 'parent', decipher the ciphertext
    using this key.
    2. Rate the fitness of the deciphered text, store the result.
    3. for(TEMP = 10;TEMP >= 0; TEMP = TEMP - STEP)
        for (count = 50,000; count>0; count--)
            Change the parent key slightly (e.g. swap two characters in the
    key at random) to get child key,
    Measure the fitness of the deciphered text using the child key
    set dF = (fitness of child - fitness of parent)
    If dF > 0 (fitness of child is higher than parent key),
    set parent = child
    If dF < 0 (fitness of child is worse than parent),
    set parent = child with probability e^(dF/T).
    */

//    vector<string> parent_key; // representation of 5x5 matrix. Each entry is a row consisting of 5 chars.
    string alfabet_string = "ABCDEFGHIKLMNOPQRSTUVWXYZ";
//    for (auto i = 0; i < alfabet_string.size(); i+=5) {
//        parent_key.push_back(alfabet_string.substr(i, 5));
//    }

    string plaintext = "";
    string ciphertext = this->getCipherText();
//    string ciphertext = "MTTBBNESWHTLMPTCUSGNBRPSOX";
    for (auto i = 0; i < ciphertext.size(); i += 2) {

        char first_letter = ciphertext[i];
        char second_letter = ciphertext[i + 1];
        int first_letter_index = alfabet_string.find(first_letter);
        int second_letter_index = alfabet_string.find(second_letter);

        if (first_letter_index / 5 == second_letter_index / 5) { // same row
            first_letter = alfabet_string[(mod(first_letter_index - 1, 5)) + (first_letter_index / 5) * 5];
            second_letter = alfabet_string[(mod(second_letter_index - 1, 5)) + (second_letter_index / 5) * 5];
        } else if (first_letter_index % 5 == second_letter_index % 5) { // same column
            first_letter = alfabet_string[((mod(first_letter_index / 5 - 1, 5)) * 5 + first_letter_index % 5)];
            second_letter = alfabet_string[((mod(second_letter_index / 5 - 1, 5)) * 5 + second_letter_index % 5)];
        } else { // different row and column
            short isFirstLetterFirst = -1;
            if (first_letter_index % 5 < second_letter_index % 5) {
                isFirstLetterFirst = 1;
            }
            first_letter = alfabet_string[((first_letter_index + isFirstLetterFirst * abs((first_letter_index % 5) - (second_letter_index % 5))) %5) + (first_letter_index / 5) * 5];
            second_letter = alfabet_string[((second_letter_index - isFirstLetterFirst * abs((first_letter_index % 5) - (second_letter_index %5))) % 5) +(second_letter_index / 5) * 5];
        }
        if (plaintext[plaintext.size() - 2] == first_letter && plaintext[plaintext.size() - 1] == 'X') {
            plaintext.pop_back();
        }
        plaintext.push_back(first_letter);
        plaintext.push_back(second_letter);
    }

    map<int, set<basic_string<char>>> freq = subStringFrequention(plaintext, 4);
    int bestFit = getFitness(freq);
    for (float TEMP = 2000; TEMP >= 0; TEMP = TEMP - 0.1f) {
        for (int count = 50000; count > 0; count--) {
            string temp_alfabet_string = modifyKey(alfabet_string);
            string temp_plaintext = decipher(temp_alfabet_string, ciphertext);
            map<int, set<basic_string<char>>> freq1 = subStringFrequention(temp_plaintext, 4);
            int temp_fit = getFitness(freq1);
            if (temp_fit >= bestFit) {
                alfabet_string = temp_alfabet_string;
                bestFit = temp_fit;
            }
            else {
                if (TEMP > 0) {
                    float prob = exp((float)(temp_fit-bestFit)/TEMP);
                    if (prob > 1.0*rand()/RAND_MAX) {
                        bestFit = temp_fit;
                        alfabet_string = temp_alfabet_string;
                    }
                }
            }
        }
    }
    return alfabet_string;

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
