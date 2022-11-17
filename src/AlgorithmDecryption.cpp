//
// Created by Maksim Karnaukh on 20/10/2022.
//

#include <sstream>
#include "AlgorithmDecryption.h"

AlgorithmDecryption::AlgorithmDecryption() = default;

AlgorithmDecryption::AlgorithmDecryption(const std::string &filename) {
    ifstream f(filename); //taking file as inputstream
    if (f) {
        ostringstream ss;
        ss << f.rdbuf(); // reading data
        this->cipherText = ss.str();
    }
    // to check
    std::cout << this->cipherText << std::endl;
}

std::map<int, std::set<std::string>>
AlgorithmDecryption::subStringFrequention(const string &text, int max_length) {
//    maps string on a frequention
    std::map<std::string, int> string_to_freq = {};
//    maps frequency to vector of strings
    std::map<int, std::set<std::string>> freq_to_string = {{0, {}}};
//    minimum length of strings to handle
    int min_length = 3;
//    memory allocation for a substring
    for (auto current_length = min_length; current_length <= max_length; current_length++) {
//        loop over string indexes
        for (auto start_index = 0; start_index + current_length < text.length(); start_index++) {
//            add a count of 1 to the map entry
            string_to_freq[text.substr(start_index, current_length)] += 1;
        }
    }
//    reverse string to frequency -> frequency to vector of strings
    for (auto it = string_to_freq.begin(); it != string_to_freq.end();it++) {
        if (it->second > 3)  {
            (freq_to_string[it->second].insert(it->first));
        }
    }
    return freq_to_string;
}

const string &AlgorithmDecryption::getCipherText() const {
    return cipherText;
}



void AlgorithmDecryption::printMap(map<int, set<std::string>> m) {
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