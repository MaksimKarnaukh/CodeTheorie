//
// Created by Maksim Karnaukh on 20/10/2022.
//

#ifndef CODETHEORIE_ENIGMA_H
#define CODETHEORIE_ENIGMA_H

#include "AlgorithmDecryption.h"
#include <algorithm>

class Enigma : public AlgorithmDecryption {
    std::array<std::array<int, 26>, 5> rotoren {}; // p0 - p4
    std::array<std::array<int, 26>, 5> inverse_rotoren {}; // p0^(-1) - p4^(-1)

    std::string crib; // known (/guessed) plaintext
    std::array<int, 26> reflector{}; // r




public:
    explicit Enigma(const std::string &filename);

    static std::array<int, 26> PermutationStringToArray(const std::string &input);

    std::string
    sendThrough(const std::string &input, const std::array<int, 3>& fast_middle_slow, const std::array<int, 3>& start_stand,
                const std::array<int, 26> &plugBoard);

    std::string Solve() override;
    static void tickRotors(std::array<int, 3>& stand_fast_middle_slow);
    static char alphabetIndex(int index);
    /**
     *
     * source: https://stackoverflow.com/questions/54970636/how-can-i-make-an-algorithm-in-c-for-finding-variations-of-a-set-without-repet , answer by MBo
     * @param n
     * @param k
     * @param idx
     * @param used
     * @param arran
     * @param comb
     */
    void GenArrangement(int n, int k, int idx, int used, int arran, std::vector<std::array<int,3>> &comb);

    /**
     * Function to check if a letter is enciphered as itself (used when performing crib dragging)
     * @param input : input string (sub-string of the ciphertext)
     * @return : false if a letter is enciphered as itself, true otherwise.
     */
    bool checkLetterCorrespondence(const std::string &input);

    /**
     * Function to make the graph.
     * @param input : input string (sub-string of the ciphertext)
     * @return :
     */
    std::map<size_t, std::pair<std::string, std::string>> makeGraph(const std::string &input);

};


#endif //CODETHEORIE_ENIGMA_H
