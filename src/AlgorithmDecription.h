//
// Created by Maksim Karnaukh on 20/10/2022.
//

#ifndef CODETHEORIE_ALGORITHMDECRIPTION_H
#define CODETHEORIE_ALGORITHMDECRIPTION_H

#include <string>
#include <fstream>
#include <iostream>

using namespace std;

/**
 * AlgorithmDecription: Base class for all the decryption classes
 */
class AlgorithmDecription {

    string cipherText; // to store the ciphertext from the file in

public:

    AlgorithmDecription();

    AlgorithmDecription(const std::string& filename);

};


#endif //CODETHEORIE_ALGORITHMDECRIPTION_H
