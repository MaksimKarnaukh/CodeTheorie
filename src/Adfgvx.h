//
// Created by Maksim Karnaukh on 20/10/2022.
//

#ifndef CODETHEORIE_ADFGVX_H
#define CODETHEORIE_ADFGVX_H
#include "AlgorithmDecryption.h"
#include <algorithm>
#include <cmath>

// morse code all characters
/*
const std::map<std::string, float> MORSE = {{".-", 'a'}, {"-...", 'b'}, {"-.-.", 'c'}, {"-..", 'd'}, {".", 'e'}, {"..-.", 'f'}, {"--.", 'g'}, {"....", 'h'},
                                            {"..", 'i'}, {".---", 'j'}, {"-.-", 'k'}, {".-..", 'l'}, {"--", 'm'}, {"-.", 'n'}, {"---", 'o'}, {".--.", 'p'},
                                            {"--.-", 'q'}, {".-.", 'r'}, {"...", 's'}, {"-", 't'}, {"..-", 'u'}, {"...-", 'v'}, {".--", 'w'}, {"-..-", 'x'},
                                            {"-.--", 'y'}, {"--..", 'z'}, {"-----", '0'}, {".----", '1'}, {"..---", '2'}, {"...--", '3'}, {"....-", '4'},
                                            {".....", '5'}, {"-....", '6'}, {"--...", '7'}, {"---..", '8'}, {"----.", '9'}};
*/
// morse code essential characters
const std::map<std::string, char> MORSE = {{".-", 'a'}, {"-..", 'd'}, {"..-.", 'f'}, {"--.", 'g'}, {"...-", 'v'}, {"-..-", 'x'}};


class Adfgvx : public AlgorithmDecryption {

    static std::string breadthFirstRead(const std::vector<std::pair<std::string, int>> &columns, int size);
    static std::map<std::string, int> twoLetterFrequency(const std::vector<std::string> &substrings);
    static std::vector<std::string> splitInSubstrings(const std::string &text);

public:
    explicit Adfgvx(const std::string &filename);
    std::string Solve() override;
    static std::string decodeMorse(const std::string &text);
};


#endif //CODETHEORIE_ADFGVX_H
