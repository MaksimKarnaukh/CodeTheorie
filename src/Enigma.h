//
// Created by Maksim Karnaukh on 20/10/2022.
//

#ifndef CODETHEORIE_ENIGMA_H
#define CODETHEORIE_ENIGMA_H

#include "AlgorithmDecryption.h"

class Enigma : public AlgorithmDecryption {
std::array<std::string,5> rotoren;
std::string crib;
std::string reflector;

public:
    explicit Enigma(const std::string &filename);
    static std::string decrypt(std::vector<std::string> rotors );
    std::string Solve() override;
};


#endif //CODETHEORIE_ENIGMA_H
