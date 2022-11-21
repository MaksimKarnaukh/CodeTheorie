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

using namespace std;

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
const std::vector<double> LETTER_FREQUENCY_EN = {	0.319119, 0.0582987, 0.108704, 0.166183, 0.496321, 0.0870574, 0.0787345, 0.238118, 0.272191, 0.00597836, 0.0301653, 0.157274, 0.0940126, 0.263712, 0.29333, 0.0753742, 0.00371205, 0.233937, 0.247223, 0.353856, 0.107767, 0.0382146, 0.0922152, 0.00586113, 0.0771325, 0.00289149,
};

const std::vector<double> LETTER_FREQUENCY_FR = {	0.278713, 0.0326682, 0.121282, 0.133029, 0.606083, 0.0386507, 0.0313992, 0.0267219, 0.274797, 0.022226, 0.00268307, 0.197822, 0.107613, 0.257248, 0.210983, 0.0914056, 0.049383, 0.242673, 0.288176, 0.262651, 0.233101, 0.0666416, 0.00177663, 0.015482, 0.00464098, 0.01182,
};
const std::vector<double> LETTER_FREQUENCY_GE = {	0.262472, 0.0811391, 0.101082, 0.187808, 0.606638, 0.0612706, 0.11133, 0.169345, 0.242344, 0.00991577, 0.0524278, 0.127166, 0.0937558, 0.361704, 0.112366, 0.0247894, 0.000665985, 0.259105, 0.268984, 0.227693, 0.190953, 0.0313013, 0.0710754, 0.00125797, 0.00144297, 0.041957,
};
const std::vector<double> LETTER_FREQUENCY_SP = {	0.445485, 0.0820444, 0.148865, 0.185572, 0.467227, 0.0256319, 0.0654874, 0.0260394, 0.258245, 0.0182609, 0.000407444, 0.18398, 0.116936, 0.260135, 0.352254, 0.0929713, 0.0324844, 0.254504, 0.295471, 0.171571, 0.115084, 0.0421519, 0.000629686, 0.00796368, 0.0373367, 0.0172979,


};
const std::vector<double> LETTER_FREQUENCY_DU = {	0.265632, 0.0560346, 0.0439765, 0.210307, 0.670642, 0.0287266, 0.120581, 0.0844066, 0.230522, 0.0517788, 0.0797961, 0.12661, 0.0783775, 0.355713, 0.214918, 0.05568, 0.000319184, 0.22733, 0.132284, 0.240807, 0.0705752, 0.101075, 0.0539067, 0.00127674, 0.00124127, 0.0492963,
};
const std::map<std::string, std::vector<double>> LETTER_FREQUENCIES = {{"EN", LETTER_FREQUENCY_EN},
                                                                       {"DU", LETTER_FREQUENCY_DU},
                                                                       {"FR", LETTER_FREQUENCY_FR},
                                                                       {"SP", LETTER_FREQUENCY_SP},
                                                                       {"GE", LETTER_FREQUENCY_GE}};
const int ASCII_A = {'A'};

/**
 * AlgorithmDecryption: Base class for all the decryption classes
 */
class AlgorithmDecryption {

    // to store the ciphertext from the file in
    string cipherText;

public:

    const string &getCipherText() const;

    AlgorithmDecryption();

    explicit AlgorithmDecryption(const std::string &filename);

    virtual ~AlgorithmDecryption() = default;

    static void vectorWithRange(std::vector<int> &v, int start, int end);

    virtual std::string Solve() = 0;

    static std::map<int, std::set<std::string>> subStringFrequention(const string &text, size_t max_length);

    static void printMap(const map<int, set<std::string>> &m);

    static double compareFrequencies(const vector<double> &left, const vector<double> &right);

    static double compareLanguageFrequencies(const vector<double> &toCompare);

    static void normalize(vector<double> &vector1);

    static void getAlphabetFrequencies(const string &string1, vector<double> &frequencies);

};


#endif //CODETHEORIE_ALGORITHMDECRYPTION_H
