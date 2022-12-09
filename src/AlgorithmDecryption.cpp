//
// Created by Maksim Karnaukh on 20/10/2022.
//

#include "AlgorithmDecryption.h"

AlgorithmDecryption::AlgorithmDecryption() = default;

AlgorithmDecryption::AlgorithmDecryption(const std::string &filename) {
    std::ifstream f(filename); //taking file as inputstream
    if (f) {
        std::ostringstream ss;
        ss << f.rdbuf(); // reading data
        this->cipherText = ss.str();
    }
    // to check
    std::cout << this->cipherText << std::endl;
}

std::map<int, std::set<std::string>>
AlgorithmDecryption::subStringFrequention(const std::string &text, size_t max_length) {
//    maps string on a frequention
    std::map<std::string, int> string_to_freq = {};
//    maps frequency to vector of strings
    std::map<int, std::set<std::string>> freq_to_string = {{0, {}}};
//    minimum length of strings to handle
    int min_length = 3;
//    memory allocation for a substring
    for (size_t current_length = min_length; current_length <= max_length; current_length++) {
//        loop over string indexes
        for (size_t start_index = 0, textLength = text.length();
             start_index + current_length < textLength; start_index++) {
//            add a count of 1 to the map entry
            string_to_freq[text.substr(start_index, current_length)] += 1;
        }
    }
//    reverse string to frequency -> frequency to vector of strings
    for (auto & it : string_to_freq) {
        if (it.second > 3) {
            (freq_to_string[it.second].insert(it.first));
        }
    }
    return freq_to_string;
}

const std::string &AlgorithmDecryption::getCipherText() const {
    return cipherText;
}


void AlgorithmDecryption::printMap(const std::map<int, std::set<std::string>>& m) {
    for (const auto &[key, elem]: m) {
        std::cout << key << std::endl;
        for (const auto &str: elem) {
            std::cout << "\t" << str << std::endl;
        }
    }
}

void AlgorithmDecryption::vectorWithRange(std::vector<int> &v, int start, int end) {
    v = {};
    for (int i = start; i < end; i++) {
        v.emplace_back(i);
    }
}

double AlgorithmDecryption::compareLanguageFrequencies(const std::vector<double> &toCompare) {
    double bestVal = std::numeric_limits<double>::max();
    for (const auto &[language, frequency]: LETTER_FREQUENCIES) {
        bestVal = std::min(bestVal, compareFrequencies(frequency, toCompare));
    }
    return bestVal;
}

double AlgorithmDecryption::compareFrequencies(const std::vector<double> &left, const std::vector<double> &right) {
    double two_norm{};
    double diff;
    for (size_t index = 0, leftSize = left.size(), rightSize = right.size() ; index < leftSize && index < rightSize; index++) {
        diff = left[index] - right[index];
        two_norm += diff * diff;
    }
    return std::sqrt(two_norm);
}

void AlgorithmDecryption::normalize(std::vector<double> &vector1) {
    double total_frequencies {0};

    for (const double &frequency: vector1) {
        total_frequencies += frequency;
    }
    for (double &elem: vector1) {
        elem /= total_frequencies;
    }

}

void AlgorithmDecryption::getAlphabetFrequencies(const std::string &string1, std::vector<double> &frequencies) {
    frequencies = std::vector<double>(26, 0.0);

    for (const char &c: string1) {
        frequencies[(int) c - ASCII_A] += 1.0;
    }
    normalize(frequencies);
}

