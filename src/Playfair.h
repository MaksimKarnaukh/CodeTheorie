//
// Created by Maksim Karnaukh on 20/10/2022.
//

#ifndef CODETHEORIE_PLAYFAIR_H
#define CODETHEORIE_PLAYFAIR_H

#include "AlgorithmDecryption.h"

class Playfair : public AlgorithmDecryption {

public:
    Playfair(const std::string& filename);

    std::string Solve() override;

    bool hasBetterFitness(const map<int, set<basic_string<char>>>& freq1, const map<int, set<basic_string<char>>>& freq2) const;

    int getFitness(const map<int, set<basic_string<char>>>& freq1) const;

    int mod(int a, int b);

    string modifyKey(string key);

    string decipher(string key, string ciphertext);


};


#endif //CODETHEORIE_PLAYFAIR_H
