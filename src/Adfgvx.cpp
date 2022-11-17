//
// Created by Maksim Karnaukh on 20/10/2022.
//

#include "Adfgvx.h"


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

Adfgvx::Adfgvx(const std::string &filename) : AlgorithmDecryption(filename) {

}

std::string Adfgvx::decrypt() {
    auto text = decodeMorse(getCipherText());
    std::cout << text << std::endl;

    return "";
}

std::string Adfgvx::decodeMorse(const std::string& text) {
    std::string currentChar;
    std::string out;

    for (const char c: text) {
        if (c == '/' || (c != '.' && c != '-')) {
            out += MORSE.at(currentChar);
            currentChar.clear();
            continue;
        }

        currentChar += c;
    }
    return out;
}