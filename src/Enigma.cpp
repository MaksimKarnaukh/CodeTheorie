//
// Created by Maksim Karnaukh on 20/10/2022.
// https://atlas.uantwerpen.be/~ssymens/enigma.php


#include "Enigma.h"

Enigma::Enigma(const std::string &filename) : AlgorithmDecryption(filename) {
    std::ifstream f(filename); //taking file as inputstream
    std::string line;
    const std::array<int, 26> *rotor{};
    std::array<int, 26> *inverse_rotor{};
    while (std::getline(f, line)) {
        if (line.find("Rotor 0: ") != std::string::npos) rotoren[0] = PermutationStringToArray(line.substr(9));
        else if (line.find("Rotor 1: ") != std::string::npos) rotoren[1] = PermutationStringToArray(line.substr(9));
        else if (line.find("Rotor 2: ") != std::string::npos) rotoren[2] = PermutationStringToArray(line.substr(9));
        else if (line.find("Rotor 3: ") != std::string::npos) rotoren[3] = PermutationStringToArray(line.substr(9));
        else if (line.find("Rotor 4: ") != std::string::npos) rotoren[4] = PermutationStringToArray(line.substr(9));
        else if (line.find("Reflector: ") != std::string::npos) reflector = PermutationStringToArray(line.substr(11));
        else if (line.find("crib: ") != std::string::npos) crib = line.substr(6);
        else this->cipherText = line;
    }
//    inverse rotors
    for (size_t rotor_index = 0; rotor_index < 5; rotor_index++) {
        rotor = &rotoren[rotor_index];
        inverse_rotor = &inverse_rotoren[rotor_index];
        for (int image_index = 0; image_index < 26; image_index++) {
            inverse_rotor->at(rotor->at(image_index)) = image_index;
        }
    }

    std::cout << this->cipherText << std::endl;
}

std::array<int, 26> Enigma::PermutationStringToArray(const std::string &input) {
    std::array<int, 26> output{};
    for (size_t index = 0; index < 26; index++) {
        output[index] = input[index] - ASCII_A;
    }
    return output;
}

std::string Enigma::sendThrough(const std::string &input, const std::array<int, 3> &fast_middle_slow,
                                const std::array<int, 3> &start_stand_fast_mid_slow,
                                const std::array<int, 26> &plugBoard) {
    std::string output{};
    int char_code{};
    std::array<int, 3> stand_fast_middle_slow = start_stand_fast_mid_slow;
    for (char input_char: input) {
        char_code = (int) (unsigned char) input_char - ASCII_A;
        // Plugboard
        char_code = plugBoard.at(char_code);
        // three rotors
        for (int rotor_nr = 0; rotor_nr < 3; rotor_nr++) { // we go to the left
            char_code = (char_code + stand_fast_middle_slow.at(rotor_nr)) % 26;
            char_code = this->rotoren.at(fast_middle_slow.at(rotor_nr)).at(char_code);
            char_code = (char_code + 26 - stand_fast_middle_slow.at(rotor_nr)) % 26;
        }
        char_code = this->reflector.at(char_code); // reflector
        for (int rotor_nr = 2; rotor_nr >= 0; rotor_nr--) { // we go back to the right
            char_code = (char_code + stand_fast_middle_slow.at(rotor_nr)) % 26;
            char_code = this->inverse_rotoren.at(fast_middle_slow.at(rotor_nr)).at(char_code);
            char_code = (char_code + 26 - stand_fast_middle_slow.at(rotor_nr)) % 26;
        }
//        Plugboard
        char_code = plugBoard.at(char_code);
        output += alphabetIndex(char_code);
        tickRotors(stand_fast_middle_slow);
    }
    return output;
}

void Enigma::tickRotors(std::array<int, 3>& stand_fast_middle_slow) {
    stand_fast_middle_slow.at(0) += 1; // rotate the fast rotor
    if (stand_fast_middle_slow.at(0) == 26) { // if the rotor made a full rotation
        stand_fast_middle_slow.at(0) = 0;
        stand_fast_middle_slow.at(1) += 1; // rotate the middle rotor
        if (stand_fast_middle_slow.at(1) == 26) { // if the middle rotor made a full rotation
            stand_fast_middle_slow.at(1) = 0;
            stand_fast_middle_slow.at(2) = (stand_fast_middle_slow.at(2) + 1) % 26; // rotate the slow rotor
        }
    }
}


std::string Enigma::Solve() {
//    this->cipherText = "PASOPVOORSALAMANDER";
    std::string input = this->cipherText;

    // Turing-Bombe
//    std::array<int, 3> fms{0, 1, 2};
//    std::string result = sendThrough(input, fms, start_pos, plugBoard);
//    std::array<int, 26> plugBoard = PermutationStringToArray("PBMEDFLHIZKGCNOAQRSWUVTXYJ");

    std::array<int, 3> start_pos{0,0,0};
    size_t inputlength =input.length(), criblength = crib.length(), end_index = inputlength-criblength;
    std::vector<std::array<int,3>> vectorcombs{};
    std::string sub_string {};
    GenArrangement(5, 3, 0, 0, 0, vectorcombs);
    for (const std::array<int, 3>& fms:vectorcombs){ // all possible rotor positions
        do {
            for (size_t c = 0; c < end_index; c++) {
                sub_string = input.substr(c, criblength);
                if (checkLetterCorrespondence(sub_string)) {
                    break;
                }
                // now we have to find a suitable k (see course notes).
                makeGraph(sub_string);

            }
            tickRotors(start_pos);
        } while (start_pos.at(2) != 0 and start_pos.at(1) != 0 and start_pos.at(0) != 0); // loop over all possible rotor configurations
    }


    std::cout << "a";
    return std::string();
}

void Enigma::GenArrangement(int n, int k, int idx, int used, int arran, std::vector<std::array<int,3>> &comb) {
    if (idx == k) {
        comb.push_back(std::array<int, 3>({std::to_string(arran)[0], std::to_string(arran)[1], std::to_string(arran)[2]}));
        return;
    }

    for (int i = 0; i < n; i++)
        if (0 == (used & (1 << i)))
            GenArrangement(n, k, idx + 1, used | (1 << i), arran * 10 + (i + 1), comb);
}



char Enigma::alphabetIndex(int index) {
    return (char) (index + ASCII_A);
}

bool Enigma::checkLetterCorrespondence(const std::string &input) {
    for (size_t char_pos = 0; char_pos < input.length(); char_pos++) {
        if (input[char_pos] == this->crib[char_pos]) {
            return true;
        }
    }
    return false;
}

std::map<size_t, std::pair<std::string, std::string>> Enigma::makeGraph(const std::string &input) {

    std::map<size_t, std::pair<std::string, std::string>> edges{};
    char char1;
    char char2;
    for (size_t char_pos = 0; char_pos < input.length(); char_pos++) {
        char1 = input[char_pos];
        char2 = this->crib[char_pos];
        edges[char_pos] = std::make_pair(char1, char2);
    }
    return edges;
}
