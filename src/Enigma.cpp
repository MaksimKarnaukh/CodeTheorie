//
// Created by Maksim Karnaukh on 20/10/2022.
//

#include "Enigma.h"

Enigma::Enigma(const std::string &filename) : AlgorithmDecryption(filename) {
    std::ifstream f(filename); //taking file as inputstream
    std::string line;

    while (std::getline(f, line)){
        if (line.find("Rotor 0: ") != std::string::npos) rotoren[0] = line.substr(9);
        else if (line.find("Rotor 1: ") != std::string::npos) rotoren[1] = line.substr(9);
        else if (line.find("Rotor 2: ") != std::string::npos) rotoren[2] = line.substr(9);
        else if (line.find("Rotor 3: ") != std::string::npos) rotoren[3] = line.substr(9);
        else if (line.find("Rotor 4: ") != std::string::npos) rotoren[4] = line.substr(9);
        else if (line.find("Reflector: ") != std::string::npos) reflector = line.substr(11);
        else if (line.find("crib: ") != std::string::npos) crib = line.substr(6);
        else this->cipherText = line;
    }
    std::cout << this->cipherText << std::endl;
}

std::string Enigma::Solve() {
    return std::string();
}
