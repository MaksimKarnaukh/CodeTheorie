//
// Created by Maksim Karnaukh on 20/10/2022.
//

#ifndef CODETHEORIE_ADFGVX_H
#define CODETHEORIE_ADFGVX_H
#include "AlgorithmDecryption.h"
#include <algorithm>
#include <cmath>


class Adfgvx : public AlgorithmDecryption {

    std::map<std::string, std::vector<std::pair<float, char>>> languageLetterFrequencies;

    static std::string breadthFirstRead(const std::vector<std::pair<std::string, int>> &columns, int size);
    static std::map<std::string, int> twoLetterFrequency(const std::vector<std::string> &substrings);
    static std::vector<std::string> splitInSubstrings(const std::string &text);

    static void twoLetterInput(std::string &text);
    static bool userInput(std::string &text);

    static std::string substitute(const std::string &text);
    static double analyseDigraphFitness(const std::string &text);

public:
    explicit Adfgvx(const std::string &filename);
    std::string Solve() override;
    static std::string decodeMorse(const std::string &text);
};


#endif //CODETHEORIE_ADFGVX_H
