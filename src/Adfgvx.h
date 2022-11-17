//
// Created by Maksim Karnaukh on 20/10/2022.
//

#ifndef CODETHEORIE_ADFGVX_H
#define CODETHEORIE_ADFGVX_H

#include "AlgorithmDecryption.h"
#include <map>
#include <vector>

class Adfgvx : public AlgorithmDecryption {
    static string decodeMorse(const std::string& text);
public:
    Adfgvx(const string& filename);
    string decrypt();
};


#endif //CODETHEORIE_ADFGVX_H
