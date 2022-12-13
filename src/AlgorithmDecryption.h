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
#include <random>
#include <algorithm>
#include <sstream>
#include <complex>


/* letter frequency sources
 * https://en.wikipedia.org/wiki/Letter_frequency
 * English {8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966, 0.153,
 *                                                0.772, 4.025, 2.406, 6.749, 7.507, 1.929, 0.095, 5.987, 6.327, 9.056,
 *                                                2.758, 0.978, 2.360, 0.150, 1.974, 0.074}
 * French {7.636 + 0.051, 0.901, 3.260 + 0.085, 3.669,
 *                                                14.715 + 0.271 + 1.504 + 0.218 + 0.008, 1.066, 0.866, 0.737,
 *                                                7.529 + 0.045 + 0.005,
 *                                                0.613, 0.074, 5.456, 2.968, 7.095, 5.796 + 0.023, 2.521, 1.362, 6.693,
 *                                                7.948,
 *                                                7.244, 6.311 + 0.058 + 0.060, 1.838, 0.049, 0.427, 0.128, 0.326}
 * German {6.516 + 0.578, 1.886 + 0.307, 2.732, 5.076, 16.396, 1.656, 3.009,
 *                                                4.577, 6.550, 0.268,
 *                                                1.417, 3.437, 2.534, 9.776, 2.594 + 0.443, 0.670, 0.018, 7.003, 7.270,
 *                                                6.154,
 *                                                4.166 + 0.995, 0.846, 1.921, 0.034, 0.039, 1.134}
 * Spannish {11.525 + 0.502, 2.215, 4.019, 5.010, 12.181 + 0.433, 0.692, 1.768,
 *                                                0.703, 6.247 + 0.725, 0.493,
 *                                                0.011, 4.967, 3.157, 6.712 + 0.311, 8.683 + 0.827, 2.510, 0.877, 6.871,
 *                                                7.977, 4.632,
 *                                                2.927 + 0.168 + 0.012, 1.138, 0.017, 0.215, 1.008, 0.467}
 * Dutch {7.49, 1.58, 1.24, 5.93, 18.91, 0.81, 3.40, 2.38, 6.50, 1.46, 2.25,
 *                                                3.57, 2.21, 10.03, 6.06, 1.57, 0.009, 6.41, 3.73, 6.79, 1.99, 2.85,
 *                                                1.52, 0.036, 0.035, 1.39,}
 * Sum of all values
*/

//Normalized
const std::vector<double> LETTER_FREQUENCY_EN = {
        0.08167, // A
        0.01492, // B
        0.02782, // C
        0.04253, // D
        0.12702, // E
        0.02228, // F
        0.02015, // G
        0.06094, // H
        0.06966, // I
        0.00153, // J
        0.00772, // K
        0.04025, // L
        0.02406, // M
        0.06749, // N
        0.07507, // O
        0.01929, // P
        0.00095, // Q
        0.05987, // R
        0.06327, // S
        0.09056, // T
        0.02758, // U
        0.00978, // V
        0.02360, // W
        0.00150, // X
        0.01974, // Y
        0.00074  // Z
};

const std::vector<double> LETTER_FREQUENCY_FR = {
        0.0778, // a
        0.0152, // b
        0.0378, // c
        0.0456, // d
        0.1368, // e
        0.0128, // f
        0.0288, // g
        0.0478, // h
        0.0497, // i
        0.0015, // j
        0.0077, // k
        0.0435, // l
        0.0254, // m
        0.0689, // n
        0.0798, // o
        0.0159, // p
        0.0012, // q
        0.0497, // r
        0.0626, // s
        0.0870, // t
        0.0261, // u
        0.0098, // v
        0.0191, // w
        0.0007, // x
        0.0573, // y
        0.0007  // z
};
const std::vector<double> LETTER_FREQUENCY_GE = {	  0.0486, // a
  0.0129, // b
  0.0203, // c
  0.0513, // d
  0.1708, // e
  0.0158, // f
  0.0295, // g
  0.0682, // h
  0.0451, // i
  0.0027, // j
  0.0058, // k
  0.0339, // l
  0.0254, // m
  0.0728, // n
  0.0578, // o
  0.0139, // p
  0.0006, // q
  0.0498, // r
  0.0575, // s
  0.0676, // t
  0.0196, // u
  0.0018, // v
  0.0136, // w
  0.0011, // x
  0.0199, // y
  0.0018  // z
   };
const std::vector<double> LETTER_FREQUENCY_SP = {
        0.0817, // a
        0.0149, // b
        0.0371, // c
        0.0422, // d
        0.1271, // e
        0.0230, // f
        0.0203, // g
        0.0592, // h
        0.0731, // i
        0.0015, // j
        0.0077, // k
        0.0426, // l
        0.0493, // m
        0.0568, // n
        0.0150, // ñ
        0.0608, // o
        0.0610, // p
        0.0019, // q
        0.0676, // r
        0.0985, // s
        0.0571, // t
        0.0251, // u
        0.0073, // v
        0.0150, // w
        0.0015, // x
        0.0133, // y
        0.0022  // z

};
const std::vector<double> LETTER_FREQUENCY_DU = {
        0.0859, // a
        0.0175, // b
        0.0236, // c
        0.0441, // d
        0.1611, // e
        0.0129, // f
        0.0209, // g
        0.0498, // h
        0.0498, // i
        0.0035, // j
        0.0047, // k
        0.0363, // l
        0.0608, // m
        0.0937, // n
        0.0772, // o
        0.0181, // p
        0.0023, // q
        0.0571, // r
        0.0610, // s
        0.0864, // t
        0.0707, // u
        0.0142, // v
        0.0053, // w
        0.0028, // x
        0.0285, // y
        0.0073  // z

};
const std::map<std::string, std::vector<double>> LETTER_FREQUENCIES = {{"EN", LETTER_FREQUENCY_EN},
                                                                       {"DU", LETTER_FREQUENCY_DU},
                                                                       {"FR", LETTER_FREQUENCY_FR},
                                                                       {"SP", LETTER_FREQUENCY_SP},
                                                                       {"GE", LETTER_FREQUENCY_GE}};
const int ASCII_A = {'A'};
const int ASCII_Z = {'Z'};

/**
 * AlgorithmDecryption: Base class for all the decryption classes
 */
class AlgorithmDecryption {
protected:
    // to store the ciphertext from the file in
    std::string cipherText;

public:

    const std::string &getCipherText() const;

    AlgorithmDecryption();

    explicit AlgorithmDecryption(const std::string &filename);

    virtual ~AlgorithmDecryption() = default;

    static void vectorWithRange(std::vector<int> &v, int start, int end);

    virtual std::string Solve() = 0;

    static std::map<int, std::set<std::string>> subStringFrequention(const std::string &text, size_t max_length);

    static void printMap(const std::map<int, std::set<std::string>> &m);

    static double compareFrequencies(const std::vector<double> &left, const std::vector<double> &right);

    static double compareLanguageFrequencies(const std::vector<double> &toCompare);

    static void normalize(std::vector<double> &vector1);

    static void getAlphabetFrequencies(const std::string &string1, std::vector<double> &frequencies);

};


#endif //CODETHEORIE_ALGORITHMDECRYPTION_H
