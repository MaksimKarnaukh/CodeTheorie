//
// Created by Maksim Karnaukh on 20/10/2022.
//

#include "AlgorithmDecryption.h"

AlgorithmDecryption::AlgorithmDecryption() = default;

AlgorithmDecryption::AlgorithmDecryption(const std::string &filename) {
    ifstream input(filename);
    if (!input.is_open()) {
        cerr << "Error while reading file contents" << endl;
    }
    string textFromFile;
    input >> textFromFile;
    this->cipherText = textFromFile;

    // to check
    std::cout << this->cipherText << std::endl;
}

const string &AlgorithmDecryption::getCipherText() const {
    return cipherText;
}
