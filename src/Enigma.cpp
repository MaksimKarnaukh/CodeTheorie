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
                // now we have to find a suitable k (see course notes).

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
