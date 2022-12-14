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
    std::vector<Vertex*> neighbours {};
    int powered = 0;
public:
    Vertex() = default;
    void addNeighbour(Vertex* v){
        neighbours.emplace_back(v);
    }
    void powerVertex(){
        if (!isPowered()){
            powered = 1;
            for (const auto& neighbour : neighbours){
                neighbour->powerVertex();
            }
        }
    }
    void powerOffVertex(){
        if (isPowered()){
            powered = 0;
            for (const auto& neighbour : neighbours){
                neighbour->powerOffVertex();
            }
        }
    }

    bool isPowered(){
        return powered == 1;
    }

    int getPowered() const {
        return powered;
    }
};

class VertexMatrix : public std::array<Vertex, 676> {
public:
    VertexMatrix() :  std::array<Vertex, 676>() {
        for (int i1 = 0 ; i1 < 26; i1++) {
            for (int i2 = 0; i2 < 26; i2++) {
                this->at(26*i1 + i2) = Vertex();
            }
        }
        char c1, c2;
        //    A-Y
        for (int i = 0; i < 25; i++) {
            for (int j = i + 1; j < 26; j++) {
                c1 = char(ASCII_A + i);
                c2 = char(ASCII_A + j);
                addEdge(c1,c2, c2, c1);
            }
        }
    }


    const Vertex * getVertex(char c1,char c2) const{
        return &(*this)[(c1-ASCII_A)*26 + (c2-ASCII_A)];
    };
    Vertex * getVertex(char c1,char c2){
        return &(*this)[(c1-ASCII_A)*26 + (c2-ASCII_A)];
    };
    void addEdge(char row_v1,char col_v1, char row_v2,char col_v2  ){
        Vertex* v1 = getVertex(row_v1,col_v1);
        Vertex* v2 = getVertex(row_v2,col_v2);
        v1->addNeighbour(v2);
        v2->addNeighbour(v1);
    }


    int getNrPoweredOnRow(char row_chr) const {
        int nr = 0;
        int start = (row_chr-ASCII_A)*26, end = start+26;
        for (size_t index = start; index<end; index++){
            nr+= (*this)[index].getPowered();
        }
        return nr;
    }

    int getNrPoweredOnCol(char i) {
        int nr = 0;
        int start = (i-ASCII_A), end = this->size();
        for (size_t index = start; index<end; index+=26){
            nr+= (*this)[index].getPowered();
        }
        return nr;
    }

    char getTurnedOffInRow(char i) {
        int start = (i-ASCII_A)*26, end = start+26;
        for (size_t index = start; index<end; index++){
            if (!(*this)[index].isPowered()) return (char) (index%26+ASCII_A);
        }
    }
    char getTurnedOffInCol(char i) {
        int start = (i-ASCII_A), end = this->size();
        for (size_t index = start; index<end; index+=26){
            if (!(*this)[index].isPowered()) return (char) (index%26+ASCII_A);
        }
    }
};


typedef std::map<const Vertex*, std::vector<const Vertex*>> gammaEdges;
static const VertexMatrix BaseVertexMatrix {};

class EnigmaConfiguration {
    int cribIndex = -1;
    pos startPos{};
    std::string plugboard{"UNKNOWN"};
    std::array<int, 3> fms{};
public:
    void setCribIndex(int cribIndex);
    friend std::ostream &operator<<(std::ostream &os, const EnigmaConfiguration &enigmaConfiguration);
    EnigmaConfiguration();

public:
    EnigmaConfiguration(std::array<int, 3> fms, const pos &startpos);

    void setPlugBoard(std::string plugboard);
};

class Enigma : public AlgorithmDecryption {
    std::array<std::array<int, 26>, 5> rotoren{}; // p0 - p4
    std::array<std::array<int, 26>, 5> inverse_rotoren{}; // p0^(-1) - p4^(-1)

    std::string crib; // known (/guessed) plaintext
    std::array<int, 26> reflector{}; // r



public:
    explicit Enigma(const std::string &filename);

    static std::array<int, 26> PermutationStringToArray(const std::string &input);

    [[maybe_unused]] std::string
    sendThrough(const std::string &input, const std::array<int, 3> &fast_middle_slow,
                const std::array<int, 3> &start_stand,
                const std::array<int, 26> &plugBoard) const;

    std::string Solve() override;

    static void tickRotors(std::array<int, 3> &stand_fast_middle_slow);

    static char alphabetIndex(int index) ;

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
};


#endif //CODETHEORIE_ENIGMA_H
