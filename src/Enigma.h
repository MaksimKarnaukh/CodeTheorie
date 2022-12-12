//
// Created by Maksim Karnaukh on 20/10/2022.
//

#ifndef CODETHEORIE_ENIGMA_H
#define CODETHEORIE_ENIGMA_H

#include "AlgorithmDecryption.h"
#include <algorithm>
typedef std::map<size_t, std::pair<char, char>> _edges;
typedef std::pair<char,char> state;
typedef std::map<state, std::vector<state>> gammaEdges;

typedef std::array<int,3> pos;

typedef std::array<std::array<bool, 26>,26> TruthGraph;

class EnigmaConfiguration{
    int cribIndex = -1;
    pos startPos;
    std::array<int,3> fms;
public:
    int getCribIndex() const;

    void setCribIndex(int cribIndex);

    const pos &getStartPos() const;

    void setStartPos(const pos &startPos);

    const std::array<int, 3> &getFms() const;

    void setFms(const std::array<int, 3> &fms);
    friend std::ostream &operator<<(std::ostream &os, const EnigmaConfiguration &enigmaConfiguration);

    EnigmaConfiguration();

public:
    EnigmaConfiguration(std::array<int, 3> fms, const pos& startpos);
};

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
     * Function to make the crib graph.
     * @param input : input string (sub-string of the ciphertext)
     * @return : graph edges.
     */
    void makeGraph(const std::string &input, std::map<size_t, std::pair<char, char>>& graph);

    /**
     * 26x26 changing graph, only the symmetric edges.
     * @return : edges of the (gamma) graph.
     */
    static gammaEdges gammaGraph();

    /**
     *
     * @param symmetricGammaGraph : 26x26 changing graph, only the symmetric edges.
     * @param graph : crib graph
     * @return : changed gamma graph edges.
     */
    std::vector<EnigmaConfiguration>
    makeAllGammaGraphs(const gammaEdges & symmetricGammaGraph, const _edges& graph, const std::vector<std::array<int, 3>>& vectorcombs, std::array<int, 3> start_pos);

    bool makeGammaGraph(const gammaEdges & symmetricGammaGraph, const _edges& graph, const std::array<int, 3>& fms, pos& start_pos);

    static void tickRotors(std::array<int, 3> &stand_fast_middle_slow, int ticks);

    static std::array<int, 3> RotorPosPlusK(const std::array<int, 3> &start_pos, int K);

    int
    sendThrough(int char_code_in, const std::array<int, 3> &fast_middle_slow,
                const pos &start_stand_fast_mid_slow,
                const std::array<int, 26> &plugBoard);

    int sendThroughRotors(int char_code_in, const std::array<int, 3> &fast_middle_slow,
                          const pos &stand_fast_middle_slow);
};


#endif //CODETHEORIE_ENIGMA_H
