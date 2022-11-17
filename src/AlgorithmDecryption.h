//
// Created by Maksim Karnaukh on 20/10/2022.
//

#ifndef CODETHEORIE_ALGORITHMDECRYPTION_H
#define CODETHEORIE_ALGORITHMDECRYPTION_H

#include <string>
#include <fstream>
#include <iostream>

using namespace std;

/**
 * AlgorithmDecryption: Base class for all the decryption classes
 */
class AlgorithmDecryption {
    string cipherText; // to store the ciphertext from the file in

public:

    const string &getCipherText() const;

    AlgorithmDecryption();

    AlgorithmDecryption(const std::string& filename);

};


#endif //CODETHEORIE_ALGORITHMDECRYPTION_H
