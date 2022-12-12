//
// Created by Maksim Karnaukh on 20/10/2022.
// https://atlas.uantwerpen.be/~ssymens/enigma.php


#include <stack>
#include <chrono>
#include <iomanip>
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

int Enigma::sendThroughRotors(int char_code_in, const std::array<int, 3> &fast_middle_slow,
                              const pos &stand_fast_middle_slow) {
    int char_code = char_code_in;
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
    return char_code;
}

int Enigma::sendThrough(int char_code_in, const std::array<int, 3> &fast_middle_slow, const pos &stand_fast_middle_slow,
                        const std::array<int, 26> &plugBoard) {
    int char_code = char_code_in;
    // Plugboard
    char_code = plugBoard.at(char_code);
//    rotors/reflector
    sendThroughRotors(char_code, fast_middle_slow, stand_fast_middle_slow);

//        Plugboard
    char_code = plugBoard.at(char_code);
    return char_code;
}

std::string Enigma::sendThrough(const std::string &input, const std::array<int, 3> &fast_middle_slow,
                                const pos &start_stand_fast_mid_slow, const std::array<int, 26> &plugBoard) {
    std::string output{};
    int char_code{};
    std::array<int, 3> stand_fast_middle_slow = start_stand_fast_mid_slow;
    for (char input_char: input) {
        char_code = (int) (unsigned char) input_char - ASCII_A;
        char_code = sendThrough(char_code, fast_middle_slow, stand_fast_middle_slow, plugBoard);

        output += alphabetIndex(char_code);

        tickRotors(stand_fast_middle_slow);
    }
    return output;
}

void Enigma::tickRotors(pos &stand_fast_middle_slow, int ticks) {
    int &fast = stand_fast_middle_slow.at(0), &middle = stand_fast_middle_slow.at(1), &slow = stand_fast_middle_slow.at(2);

    fast += ticks; // rotate the fast rotor
    if (fast < 26) return;
    while (fast >= 26) { // if the rotor made a full rotation
        fast -= 26;
        middle += 1; // rotate the middle rotor
    }
    if (middle < 26) return;
    while (middle >= 26) { // if the middle rotor made a full rotation
        middle -= 26;
        slow += 1; // rotate the slow rotor
    }
    slow %= 26;
}

void Enigma::tickRotors(pos &stand_fast_middle_slow) {
    Enigma::tickRotors(stand_fast_middle_slow, 1);
}

pos Enigma::RotorPosPlusK(const std::array<int, 3> &start_pos, int K) {
    std::array<int, 3> newRotorPos = start_pos;
    Enigma::tickRotors(newRotorPos, K);
    return newRotorPos;
}


std::string Enigma::Solve() {
    std::string input = this->cipherText;
    std::fstream ofs;
    auto start = std::chrono::high_resolution_clock::now();
    long long diff;
    ofs.open("EnigmaValidConfigurations.txt", std::ofstream::out | std::ofstream::trunc);
    // Turing-Bombe
    pos start_pos{0, 0, 0};
    size_t inputlength = input.length(), criblength = crib.length(), end_index = inputlength - criblength;
    std::vector<std::array<int, 3>> vectorcombs{};
    std::string sub_string{};
    GenArrangement(5, 3, 0, 0, 0, vectorcombs);
    _edges edges;
    gammaEdges _gammaEdges = gammaGraph();
    std::vector<EnigmaConfiguration> valid_configurations{};
    std::cout << std::fixed;
    std::cout << std::setprecision(2);

    if (ofs.is_open()) {
        ofs << "{" << std::endl;
        for (size_t c = 0; c < end_index; c++) { // crib dragging loop
//                time diff since start
            diff = std::chrono::duration_cast<std::chrono::minutes>(
                    std::chrono::high_resolution_clock::now() - start).count();
//                Output counter, time and interations per second
            std::cout << "\t|Index: " << std::to_string(c) << "\t |Time: " << diff << "\t|Indexes/Minute: "
                      << c / ((float)(diff + 1)) << "\t |Expected Remaining Time: " << (int) std::floor(float (end_index-c)/((float)c/float(diff+1))) << std::endl;
            sub_string = input.substr(c, criblength);
            if (!checkLetterCorrespondence(sub_string)) { // if letter enciphered as itself
                std::cout << "Crib match succeed on starting pos: " << c << "/" << end_index << std::endl;
                // now we have to find a suitable k (see course notes).
                makeGraph(sub_string, edges);
                valid_configurations = makeAllGammaGraphs(_gammaEdges, edges, vectorcombs, start_pos);
                for (auto &config: valid_configurations) {
                    config.setCribIndex(c);
                    ofs << config;
                }
            } else {
                std::cout << "Crib match failed on starting pos: " << c << "/" << end_index << std::endl;
            }

        }
        ofs << "}" << std::endl;
        ofs.close();
    } else std::cout << "Unable to open file";

    std::cout << "a";
    return "std::string()";
}

void Enigma::GenArrangement(int n, int k, int idx, int used, int arran, std::vector<std::array<int, 3>> &comb) {
    if (idx == k) {
        comb.push_back(std::array<int, 3>({arran / 100 % 100, arran / 10 % 10, arran % 10}));
        return;
    }

    for (int i = 0; i < n; i++)
        if (0 == (used & (1 << i)))
            GenArrangement(n, k, idx + 1, used | (1 << i), arran * 10 + (i), comb);
}


char Enigma::alphabetIndex(int index) {
    return (char) (index + ASCII_A);
}

bool Enigma::checkLetterCorrespondence(const std::string &input) {
    for (size_t char_pos = 0, inputLength = input.length(); char_pos < inputLength; char_pos++) {
        if (input[char_pos] == crib[char_pos]) {
            return true;
        }
    }
    return false;
}

void Enigma::makeGraph(const std::string &input, std::map<size_t, std::pair<char, char>>& graph) {
    graph = {};
    char char1;
    char char2;
    for (size_t char_pos = 0, inputLength = input.length(); char_pos < inputLength; char_pos++) {
        char1 = input[char_pos];
        char2 = this->crib[char_pos];
        graph[char_pos + 1] = std::make_pair(char1, char2);
    }
}

gammaEdges Enigma::gammaGraph() {

    gammaEdges gammaSymmetricEdges{};
    state vertex1{}, vertex2{};
    char c1, c2;
//    A-Y
    for (int i = 0; i < 25; i++) {
        for (int j = i + 1; j < 26; j++) {
            c1 = char(ASCII_A + i);
            c2 = char(ASCII_A + j);
            vertex1 = std::make_pair(c1, c2);
            vertex2 = std::make_pair(c2, c1);
            gammaSymmetricEdges[vertex1].emplace_back(vertex2);
            gammaSymmetricEdges[vertex2].emplace_back(vertex1);

        }
    }

    return gammaSymmetricEdges;
}

std::vector<EnigmaConfiguration> Enigma::makeAllGammaGraphs(const gammaEdges &symmetricGammaGraph, const _edges &graph,
                                                            const std::vector<std::array<int, 3>> &vectorcombs,
                                                            pos start_pos) {

    // per rotorstand, heel het circuit opbouwen.
    bool valid{};
    std::vector<EnigmaConfiguration> valid_configurations{};
    EnigmaConfiguration tempEnigmaConfiguration;

    auto start = std::chrono::high_resolution_clock::now();
    long long diff;
    unsigned int counter {};

    for (const std::array<int, 3> &fms: vectorcombs) { // all possible rotor positions
        do {
            counter++;
            if (counter % 100000 == 0) {
//                time diff since start
                diff = std::chrono::duration_cast<std::chrono::seconds>(
                        std::chrono::high_resolution_clock::now() - start).count();
//                Output counter, time and interations per second
                std::cout << "\t|Counter: " << std::to_string(counter) << "\t |Time: "
                          << diff
                          << "\t|Iterations/Second: " << counter / (diff + 1) << std::endl;
            }

            valid = makeGammaGraph(symmetricGammaGraph, graph, fms, start_pos);
            if (valid) {
                tempEnigmaConfiguration = EnigmaConfiguration(fms, start_pos);
                valid_configurations.emplace_back(tempEnigmaConfiguration);
            }
            tickRotors(start_pos);
        } while (start_pos.at(2) != 0 or start_pos.at(1) != 0 or
                 start_pos.at(0) != 0); // loop over all possible rotor configurations
    }

    return valid_configurations;
}

bool Enigma::makeGammaGraph(const gammaEdges &symmetricGammaGraph, const _edges &graph, const std::array<int, 3> &fms,
                            pos &start_pos) {
    pos rotor_plus_k_pos{};
    std::set<int> enabled_columns{}, enabled_rows{};


    gammaEdges changedGammaGraph = symmetricGammaGraph;
    char c1{}, c2{};
    state vertex1{}, vertex2{}, maxVertex{};
    size_t maxSize{0}, vectorSize{};
    std::set<char> filled_cols{}, filled_rows{};
    std::stack<state> todo{};
    bool returnVal{};
    int rel_pos;
    char enigmaInput {}, enigmaOutput {};
    for (const auto &it: graph) {
        rel_pos = it.first;
        c1 = it.second.first;
        c2 = it.second.second;
        rotor_plus_k_pos = Enigma::RotorPosPlusK(start_pos, rel_pos);
        for (int c = 0; c < 26; c++) {
            enigmaInput = char(ASCII_A + c);
            enigmaOutput = char(ASCII_A + sendThroughRotors(c, fms, rotor_plus_k_pos));
            vertex1 = std::make_pair(c1, enigmaInput);
            vertex2 = std::make_pair(c2, enigmaOutput);
            changedGammaGraph[vertex1].emplace_back(vertex2);
            changedGammaGraph[vertex2].emplace_back(vertex1);
        }
    }
    for (const auto &[vertex, edges]: changedGammaGraph) {
        vectorSize = edges.size();
        if (maxSize < vectorSize) {
            maxVertex = vertex;
            maxSize = vectorSize;
        }
    }
    todo.push(maxVertex);
    filled_cols = {maxVertex.second};
    filled_rows = {maxVertex.first};

    while (!todo.empty()) {
        vertex1 = todo.top();
        todo.pop();
        for (const state &vertex: changedGammaGraph.at(vertex1)) {
            if (filled_rows.find(vertex.first) != filled_rows.end() ||
                filled_cols.find(vertex.second) != filled_cols.end()) {
                return false;
            }
            filled_rows.insert(vertex.first);
            filled_cols.insert(vertex.second);
            todo.push(vertex);
        }
    }
    returnVal = (filled_rows.size() == 26 && filled_cols.size() == 26);
    return returnVal;
}


EnigmaConfiguration::EnigmaConfiguration(const std::array<int, 3> fms, const pos &startpos) : startPos(startpos),
                                                                                              fms(fms) {

}

int EnigmaConfiguration::getCribIndex() const {
    return cribIndex;
}

void EnigmaConfiguration::setCribIndex(int cribIndex) {
    EnigmaConfiguration::cribIndex = cribIndex;
}

const pos &EnigmaConfiguration::getStartPos() const {
    return startPos;
}

void EnigmaConfiguration::setStartPos(const pos &startPos) {
    EnigmaConfiguration::startPos = startPos;
}

const std::array<int, 3> &EnigmaConfiguration::getFms() const {
    return fms;
}

void EnigmaConfiguration::setFms(const std::array<int, 3> &fms) {
    EnigmaConfiguration::fms = fms;
}

std::ostream &operator<<(std::ostream &os, const EnigmaConfiguration &enigmaConfiguration) {
    os << '{' << std::endl;
    os << "\tfms:" << std::endl;
    os << "\t{" << std::endl;
    os << "\t\t{";
    for (int i: enigmaConfiguration.fms) {
        os << i << ',';
    }
    os << '}' << std::endl;

    os << "\trotorStartPos:" << std::endl;
    os << "\t{" << std::endl;
    os << "\t\t{";
    for (int i: enigmaConfiguration.startPos) {
        os << i << ',';
    }
    os << "\t}" << std::endl;

    os << "\tcribIndex:" << std::endl;
    os << "\t{" << std::endl;
    os << "\t\t" << enigmaConfiguration.cribIndex;

    os << "\t}" << std::endl;


    os << '}' << std::endl;

    return os;
}

EnigmaConfiguration::EnigmaConfiguration() {

}
