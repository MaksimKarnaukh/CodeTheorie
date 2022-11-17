//
// Created by Maksim Karnaukh on 20/10/2022.
//

#ifndef CODETHEORIE_ALGORITHMDECRYPTION_H
#define CODETHEORIE_ALGORITHMDECRYPTION_H

#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <streambuf>
#include <limits>

using namespace std;

/* letter frequency sources
 * English, French, German: https://www.researchgate.net/figure/Relative-frequencies-of-letters-in-English-French-and-German-languages_tbl1_337312662
 * Dutch: https://www.sttmedia.com/characterfrequency-dutch#letters
 * Spanish: http://practicalcryptography.com/cryptanalysis/letter-frequencies-various-languages/spanish-letter-frequencies/
 * Sum of all values
 * EN: 99.95
 * DU: 99.99
 * FR: 96.39    <- not good
 * SP: 100
 * GE: 98.15    <- is okay
 */
const std::vector<float> LETTER_FREQUENCY_EN = {8.34, 1.54, 2.73, 4.14, 12.60, 2.03, 1.92, 6.11, 6.71, 0.23, 0.87, 4.24,
                                                2.53,
                                                6.80, 7.70, 1.66, 0.09, 5.68, 6.11, 9.37, 2.85, 1.06, 2.34, 0.20, 2.04,
                                                0.06};
const std::vector<float> LETTER_FREQUENCY_DU = {7.79, 1.38, 1.31, 5.48, 19.34, 0.74, 3.17, 3.16, 5.71, 1.18, 2.83, 3.85,
                                                2.60,
                                                10.04, 5.89, 1.51, 0.01, 5.70, 3.91, 6.50, 2.15, 2.27, 1.74, 0.05, 0.06,
                                                1.62};
const std::vector<float> LETTER_FREQUENCY_FR = {8.13, 0.93, 3.15, 3.55, 15.10, 0.96, 0.97, 1.08, 6.94, 0.71, 0.16, 5.68,
                                                3.23,
                                                6.42, 5.27, 3.03, 0.89, 6.43, 7.91, 7.11, 6.05, 1.83, 0.04, 0.42, 0.19,
                                                0.21};
const std::vector<float> LETTER_FREQUENCY_SP = {12.50, 1.27, 4.43, 5.14, 13.24, 0.79, 1.17, 0.81, 6.91, 0.45, 0.08,
                                                5.84, 2.61,
                                                7.31, 8.98, 2.75, 0.83, 6.62, 7.44, 4.42, 4.00, 0.98, 0.03, 0.19, 0.79,
                                                0.42};
const std::vector<float> LETTER_FREQUENCY_GE = {5.58, 1.96, 3.16, 4.98, 16.93, 1.49, 3.02, 4.98, 8.02, 0.24, 1.32, 3.60,
                                                2.55,
                                                10.53, 2.24, 0.67, 0.02, 6.89, 6.42, 5.79, 3.83, 0.84, 1.78, 0.05, 0.05,
                                                1.21};
const std::map<std::string, std::vector<float>> LETTER_FREQUENCIES = {{"EN", LETTER_FREQUENCY_EN},
                                                                      {"DU", LETTER_FREQUENCY_DU},
                                                                      {"FR", LETTER_FREQUENCY_FR},
                                                                      {"SP", LETTER_FREQUENCY_SP},
                                                                      {"GE", LETTER_FREQUENCY_GE}};

/**
 * AlgorithmDecryption: Base class for all the decryption classes
 */
class AlgorithmDecryption {

    // to store the ciphertext from the file in
    string cipherText;

public:

    const string &getCipherText() const;

    AlgorithmDecryption();

    AlgorithmDecryption(const std::string &filename);

    virtual ~AlgorithmDecryption() = default;

    void vectorWithRange(std::vector<int> &v, int start, int end);

    virtual std::string Solve() = 0;

    static std::map<int, std::set<std::string>> subStringFrequention(const string &text, int max_length);

    static void printMap(map<int, set<std::string>> m);

    static float compareFrequencies(const vector<float> &left, const vector<float> &right);
    static float compareLanguageFrequencies(const vector<float> &toCompare);

};


#endif //CODETHEORIE_ALGORITHMDECRYPTION_H
