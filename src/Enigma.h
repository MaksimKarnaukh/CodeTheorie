//
// Created by Maksim Karnaukh on 20/10/2022.
//

#ifndef CODETHEORIE_ENIGMA_H
#define CODETHEORIE_ENIGMA_H

#include <thread>
#include <mutex>

#include "AlgorithmDecryption.h"
#include <algorithm>

typedef std::map<size_t, std::pair<char, char>> _edges;
typedef std::array<int, 3> pos;

class Vertex {
    std::vector<Vertex *> neighbours{};
    int powered = 0;
public:
    Vertex() = default;

    void addNeighbour(Vertex *v);

    void powerVertex();

    void powerOffVertex();

    bool isPowered() const;

    int getPowered() const;
};

class VertexMatrix : public std::array<Vertex, 676> {
public:
    VertexMatrix();


    const Vertex *getVertex(char c1, char c2) const {
        return &(*this)[(c1 - ASCII_A) * 26 + (c2 - ASCII_A)];
    };

    Vertex *getVertex(char c1, char c2);;

    void addEdge(char row_v1, char col_v1, char row_v2, char col_v2);


    int getNrPoweredOnRow(char row_chr) const;

    int getNrPoweredOnCol(char i);

    char getTurnedOffInRow(char i);

    char getTurnedOffInCol(char i);
};


typedef std::map<const Vertex *, std::vector<const Vertex *>> gammaEdges;
static const VertexMatrix BaseVertexMatrix{};

class EnigmaConfiguration {
public:
    int cribIndex = -1;
    pos startPos{};
    std::string plugboard{"UNKNOWN"};
    std::array<int, 3> fms{};
    std::array<std::set<char>, 26> possible_characters{};
    void setCribIndex(int cribIndex);

    friend std::ostream &operator<<(std::ostream &os, const EnigmaConfiguration &enigmaConfiguration);

    EnigmaConfiguration();

public:
    EnigmaConfiguration(std::array<int, 3> fms, const pos &startpos);

    void setPlugBoard(std::string plugboard);

    void setPlugboardPossibilitites(std::array<std::set<char>, 26> array1);
};

class Enigma : public AlgorithmDecryption {
    std::array<std::array<int, 26>, 5> rotoren{}; // p0 - p4
    std::array<std::array<int, 26>, 5> inverse_rotoren{}; // p0^(-1) - p4^(-1)

    std::string crib; // known (/guessed) plaintext
    std::array<int, 26> reflector{}; // r



public:
    explicit Enigma(const std::string &filename);

    static std::array<int, 26> PermutationStringToArray(const std::string &input);

    std::string
    sendThrough(const std::string &input, const std::array<int, 3> &fast_middle_slow,
                const std::array<int, 3> &start_stand,
                const std::array<int, 26> &plugBoard) const;

    std::string Solve() override;

    static void tickRotors(std::array<int, 3> &stand_fast_middle_slow);

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
    static void GenArrangement(int n, int k, int idx, int used, int arran, std::vector<std::array<int, 3>> &comb);

    /**
     * Function to check if a letter is enciphered as itself (used when performing crib dragging)
     * @param input : input string (sub-string of the ciphertext)
     * @return : false if a letter is enciphered as itself, true otherwise.
     */
    bool checkLetterCorrespondence(const std::string &input) const;

    /**
     * Function to make the crib graph.
     * @param input : input string (sub-string of the ciphertext)
     * @return : vertice with most edges.
     */
    char makeGraph(const std::string &input, std::map<size_t, std::pair<char, char>> &graph) const;

    /**
     * 26x26 changing graph, only the symmetric edges.
     * @return : edges of the (gamma) graph.
     */
    static VertexMatrix gammaGraph();

    /**
     *
     * @param vertexMatrix : 26x26 changing graph, only the symmetric edges.
     * @param graph : crib graph
     * @return : changed gamma graph edges.
     */
    std::vector<EnigmaConfiguration>
    makeAllGammaGraphs(const _edges &graph, const std::vector<std::array<int, 3>> &vectorcombs,
                       char charWithMostEdges) const;

    bool makeGammaGraph(const _edges &graph, const std::array<int, 3> &fms, pos start_pos,
                        std::vector<EnigmaConfiguration> &validConfigurations, std::mutex &valids_mutex,
                        char charWithMostEdges) const;

    static void tickRotors(std::array<int, 3> &stand_fast_middle_slow, int ticks);

    static std::array<int, 3> RotorPosPlusK(const std::array<int, 3> &start_pos, int K);

    int
    sendThrough(int char_code_in, const std::array<int, 3> &fast_middle_slow,
                const pos &start_stand_fast_mid_slow,
                const std::array<int, 26> &plugBoard) const;

    int sendThroughRotors(int char_code_in, const std::array<int, 3> &fast_middle_slow,
                          const pos &stand_fast_middle_slow) const;

    std::string
    solvePlugBoard(std::array<std::set<char>, 26> possibilities, std::string plugboard, const std::array<int, 3> &fms,
                   const pos &startpos) const;

    void
    _solvePlugBoard(std::array<std::set<char>, 26> possibilities, std::string plugboard, const std::array<int, 3> &fms,
                    const pos &startpos, float& bestFitness, std::string &bestPlugBoard) const;
};


#endif //CODETHEORIE_ENIGMA_H
