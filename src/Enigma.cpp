//
// Created by Maksim Karnaukh on 20/10/2022.
// https://atlas.uantwerpen.be/~ssymens/enigma.php


#include <stack>
#include <chrono>
#include <iomanip>
#include <future>
#include "Enigma.h"

Enigma::Enigma(const std::string &filename) : AlgorithmDecryption(filename) {
    std::ifstream f(filename); //taking file as inputstream
    std::string line;
    const std::array<int, 26> *rotor;
    std::array<int, 26> *inverse_rotor;
    while (std::getline(f, line)) {
        if (line.find("Rotor 0: ") != std::string::npos) rotoren[0] = PermutationStringToArray(line.substr(9));
        else if (line.find("Rotor 1: ") != std::string::npos) rotoren[1] = PermutationStringToArray(line.substr(9));
        else if (line.find("Rotor 2: ") != std::string::npos) rotoren[2] = PermutationStringToArray(line.substr(9));
        else if (line.find("Rotor 3: ") != std::string::npos) rotoren[3] = PermutationStringToArray(line.substr(9));
        else if (line.find("Rotor 4: ") != std::string::npos) rotoren[4] = PermutationStringToArray(line.substr(9));
        else if (line.find("Reflector: ") != std::string::npos) reflector = PermutationStringToArray(line.substr(11));
        else if (line.find("crib: ") != std::string::npos) crib = line.substr(6);
        else if (line.find(":") == std::string::npos)  this->cipherText = line;
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
                              const pos &stand_fast_middle_slow) const {
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
                        const std::array<int, 26> &plugBoard) const {
    int char_code = char_code_in;
    // Plugboard
    char_code = plugBoard.at(char_code);
//    rotors/reflector
    char_code = sendThroughRotors(char_code, fast_middle_slow, stand_fast_middle_slow);

//        Plugboard
    char_code = plugBoard.at(char_code);
    return char_code;
}

std::string Enigma::sendThrough(const std::string &input, const std::array<int, 3> &fast_middle_slow,
                                                 const pos &start_stand_fast_mid_slow,
                                                 const std::array<int, 26> &plugBoard) const {
    std::string output{};
    int char_code;
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
    int &fast = stand_fast_middle_slow.at(0), &middle = stand_fast_middle_slow.at(1), &slow = stand_fast_middle_slow.at(
            2);

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

//    input = "DAEDAQOZSIQMMKBILGMPWHAIV";
//    this->crib = "KEINEZUSAETZEZUMVORBERIQT";

    std::fstream ofs;
    auto start = std::chrono::high_resolution_clock::now();
    long long diff;
    ofs.open("EnigmaValidConfigurations.txt", std::ofstream::out | std::ofstream::trunc);
    // Turing-Bombe
    size_t inputlength = input.length(), criblength = crib.length(), end_index = inputlength - criblength;
    std::vector<std::array<int, 3>> vectorcombs{};
    std::string sub_string{};
    GenArrangement(5, 3, 0, 0, 0, vectorcombs);
    _edges edges;
    VertexMatrix vertexMatrix = gammaGraph();
    std::vector<EnigmaConfiguration> valid_configurations{};
    std::cout << std::fixed;
    std::cout << std::setprecision(2);

    if (ofs.is_open()) {
        ofs << "{" << std::endl;
        for (size_t c = 0; c <= end_index; c++) { // crib dragging loop
//                time diff since start
            diff = std::chrono::duration_cast<std::chrono::minutes>(
                    std::chrono::high_resolution_clock::now() - start).count();
//                Output counter, time and interations per second
            std::cout << "|Index: " << std::to_string(c) << "\t |Time (min): " << diff << "\t|Indexes/Minute: "
                      << (float) c / ((float) (diff + 1)) << "\t |Expected Remaining Time (min): "
                      << (int) std::ceil((float) (end_index - c) / ((float) c / float(diff + 1))) << std::endl;
            sub_string = input.substr(c, criblength);
            if (!checkLetterCorrespondence(sub_string)) { // if letter enciphered as itself
                std::cout << "Crib match succeed on starting pos: " << c << "/" << end_index << std::endl;
                // now we have to find a suitable k (see course notes).
                char charWithMostEdges = makeGraph(sub_string, edges);
                valid_configurations = makeAllGammaGraphs(edges, vectorcombs, charWithMostEdges);
                if(!valid_configurations.empty()){
                    for (auto &config: valid_configurations) {
                        config.setCribIndex((int) c);
                        ofs << config;
                    }
                    return sendThrough(cipherText, valid_configurations[0].fms,valid_configurations[0].startPos,Enigma::PermutationStringToArray(valid_configurations[0].plugboard));
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

bool Enigma::checkLetterCorrespondence(const std::string &input) const {
    for (size_t char_pos = 0, inputLength = input.length(); char_pos < inputLength; char_pos++) {
        if (input[char_pos] == crib[char_pos]) {
            return true;
        }
    }
    return false;
}

char Enigma::makeGraph(const std::string &input, std::map<size_t, std::pair<char, char>> &graph) const {
    graph = {};
    char char1;
    char char2;
    std::map<char, int> charCounts{};
    for (size_t char_pos = 0, inputLength = input.length(); char_pos < inputLength; char_pos++) {
        char1 = input[char_pos];
        char2 = this->crib[char_pos];
        graph[char_pos + 1] = std::make_pair(char2, char1);
        charCounts[char2] += 1;
        charCounts[char1] += 1;
    }
    auto it = std::max_element(charCounts.begin(), charCounts.end(),
                               [](const auto &a, const auto &b) { return a.second < b.second; });
    return it->first;
}

VertexMatrix Enigma::gammaGraph() {

    VertexMatrix vertexMatrix{};
    const Vertex *vertex1{}, *vertex2{};
    char c1, c2;
//    A-Y
    for (int i = 0; i < 25; i++) {
        for (int j = i + 1; j < 26; j++) {
            c1 = char(ASCII_A + i);
            c2 = char(ASCII_A + j);
            vertexMatrix.addEdge(c1, c2, c2, c1);
        }
    }

    return vertexMatrix;
}



std::vector<EnigmaConfiguration>
Enigma::makeAllGammaGraphs(const _edges &graph, const std::vector<std::array<int, 3>> &vectorcombs,
                           char charWithMostEdges) const {
    pos start_pos{0, 0, 0};

    // per rotorstand, heel het circuit opbouwen.
    std::vector<EnigmaConfiguration> valid_configurations{};
    EnigmaConfiguration tempEnigmaConfiguration;
    std::vector<std::thread> threads{};
    int nr_threads = 10000;
    std::mutex valids_mutex{};


    auto start = std::chrono::high_resolution_clock::now();
    long long diff;
    unsigned int counter{};

    for (const std::array<int, 3> &fms: vectorcombs) { // all possible rotor positions
        do {
            counter++;
            if (counter % 1000 == 0) {
//                time diff since start
                diff = std::chrono::duration_cast<std::chrono::seconds>(
                        std::chrono::high_resolution_clock::now() - start).count();
//                Output counter, time and interations per second
//                std::cout << "\t|Counter: " << std::to_string(counter) << "\t |Time (s): "
//                          << diff
//                          << "\t|Iterations/Second: " << counter / (diff + 1) << std::endl;
            }
//            threads.emplace_back(
//                    &Enigma::makeGammaGraph, this, std::cref(graph),
//                            std::cref(fms), start_pos, std::ref(valid_configurations),
//                            std::ref(valids_mutex), charWithMostEdges);
            if (this->makeGammaGraph(graph,
                                 fms,start_pos, valid_configurations,
                                 valids_mutex, charWithMostEdges)){
                for (auto &thread: threads) {
                    thread.join();
                }
                threads.clear();
                return valid_configurations;
            }
//            this->makeGammaGraph( graph,
//                                  fms,start_pos, valid_configurations,
//                                 valids_mutex, charWithMostEdges);
//

            if (counter % nr_threads == 0) {
                for (auto &thread: threads) {
                    thread.join();
                }
                threads.clear();
            }
            tickRotors(start_pos);
        } while (start_pos.at(2) != 0 or start_pos.at(1) != 0 or
                 start_pos.at(0) != 0); // loop over all possible rotor configurations
    }
    for (auto &thread: threads) {
        thread.join();
    }
    return valid_configurations;
}

bool Enigma::makeGammaGraph(const _edges &graph, const std::array<int, 3> &fms, pos start_pos,
                            std::vector<EnigmaConfiguration> &validConfigurations, std::mutex &valids_mutex,
                            char charWithMostEdges) const {

    pos rotor_plus_k_pos{};
    std::set<int> enabled_columns{}, enabled_rows{};
    VertexMatrix adjecencyMatrix = VertexMatrix();
    char c1, c2,c3;
    int nr_r, idx;
    std::set<char> filled_cols{}, filled_rows{};
    size_t rel_pos;
    char enigmaInput, enigmaOutput;
    for (const auto &it: graph) {
        rel_pos = it.first - 1;
        c1 = it.second.first;
        c2 = it.second.second;
        rotor_plus_k_pos = Enigma::RotorPosPlusK(start_pos, rel_pos);
        for (int c = 0; c < 26; c++) {
            enigmaInput = (char) (ASCII_A + c);
            enigmaOutput = (char) (ASCII_A + sendThroughRotors(c, fms, rotor_plus_k_pos));
            adjecencyMatrix.addEdge(c1, enigmaInput, c2, enigmaOutput);
        }
    }

    adjecencyMatrix.getVertex(charWithMostEdges, 'A')->powerVertex();
    nr_r = adjecencyMatrix.getNrPoweredOnRow(charWithMostEdges);

    if (nr_r == 26) return false;
    else {
        adjecencyMatrix.getVertex(charWithMostEdges, 'A')->powerOffVertex();
        EnigmaConfiguration temp = EnigmaConfiguration(fms, start_pos);
        std::string plugBoard = std::string(26, ' ');
        std::array<std::set<char>, 26> possibilitties {};
        bool done = false;
        for (c1 = ASCII_A; c1 <= ASCII_Z; c1++) {
            for (c2 = ASCII_A; c2 <= ASCII_Z; c2++) {
                adjecencyMatrix.getVertex(c1, c2)->powerVertex();
                //            nr_c = adjecencyMatrix.getNrPoweredOnCol(c);
                if (adjecencyMatrix.getNrPoweredOnRow(c1) == 25) {
                    c3 = adjecencyMatrix.getTurnedOffInRow(c1);
                    idx = c1 - ASCII_A;
                    plugBoard[idx] = c3;
                    adjecencyMatrix.getVertex(c1, c2)->powerOffVertex();
                    break;
                } else if (adjecencyMatrix.getNrPoweredOnCol(c1) == 25) {
                    c3 = adjecencyMatrix.getTurnedOffInCol(c1);
                    idx = c1 - ASCII_A;
                    plugBoard[idx] = c3;
                    adjecencyMatrix.getVertex(c1, c2)->powerOffVertex();
                    break;
                }else if (adjecencyMatrix.getNrPoweredOnCol(c1) == 1 && adjecencyMatrix.getNrPoweredOnRow(c1) == 1) {
                    possibilitties[c1-ASCII_A].insert(c2);
                }
                adjecencyMatrix.getVertex(c1, c2)->powerOffVertex();

            }
        }
        plugBoard = solvePlugBoard(possibilitties, plugBoard,fms,start_pos);
        temp.setPlugBoard(plugBoard);
        temp.setPlugboardPossibilitites(possibilitties);
        std::cout << "%%%%%%%%%%%%%SUCCESSFUL%%%%%%%%%%%%%" << std::endl;
        std::cout << temp;
        std::cout << "%%%%%%%%%%%%%SUCCESSFUL%%%%%%%%%%%%%" << std::endl;
        std::lock_guard<std::mutex> guard(valids_mutex);
        validConfigurations.emplace_back(temp);
        return true;
    }
    return false;
}

std::string Enigma::solvePlugBoard(std::array<std::set<char>, 26> possibilities, std::string plugboard,const std::array<int,3>& fms,const pos& startpos) const {
    bool found {false};
    std::map<char,std::vector<int>> occurences {};
    for (int idx = 0; idx<26;idx++){
        if (plugboard[idx] != ' '){
            possibilities[idx] = {};
            for (int i2 = 0; i2 < 26; i2++) {
                if (possibilities[i2].find(plugboard[idx])!=possibilities[i2].end()){
                    possibilities[i2].erase(possibilities[i2].find(plugboard[idx]));
                }
            }
        }
    }
    for (int idx = 0; idx<26;idx++) {
        for (char c : possibilities[idx]){
            if (occurences.find(c) == occurences.end()){
                occurences[c] = {};
            }
            occurences[c].emplace_back(idx);
        }
    }
    for (int idx = 0; idx<26;idx++) {
        if (occurences.find(idx+ASCII_A) != occurences.end() && occurences[idx+ASCII_A].size() == 1){
            plugboard[occurences[idx+ASCII_A][0]] = idx+ASCII_A;
            found= true;
        }
    }
    if (found) return solvePlugBoard(possibilities, plugboard,fms,startpos);
    float fitness = 1.0f;
    std::string bestplugboard {};
    _solvePlugBoard(possibilities,plugboard,fms,startpos, fitness,bestplugboard);
    return bestplugboard;
}
void Enigma::_solvePlugBoard(std::array<std::set<char>, 26> possibilities, std::string plugboard,const std::array<int,3>& fms,const pos& startpos, float& bestFitness, std::string& bestPlugBoard ) const {
    bool done;
    std::string resultingPlugboard, nextplugboard;
    std::array<std::set<char>, 26> cp;
    done= true;
    for (const char & c : plugboard) {
        if(c==' ' ){
            done = false;
            break;
        }
    }
    if (done){
        std::string plaintext = sendThrough(this->cipherText,fms,startpos,Enigma::PermutationStringToArray(plugboard));
        std::vector<double> freq {};
        double fitness;
        if(plaintext.find(this->crib)!=std::string::npos){
            this->getAlphabetFrequencies(plaintext,freq);
            fitness = this->compareFrequencies(freq,LETTER_FREQUENCY_EN);
            if (fitness<bestFitness){
                bestFitness=fitness;
                bestPlugBoard = std::string(plugboard);
            }
            return;
        }
    }
    for (int i = 0; i < 26; i++) {
        if (possibilities[i].size() >= 1) {
            for (char guess : possibilities[i]){
                if(plugboard.find(guess) == std::string::npos){
                    cp = possibilities;
                    nextplugboard = plugboard;
                    cp[i] = {};
                    nextplugboard[i] = guess;
                    for (int i2 = 0; i2 < 26; i2++) {
                        if (cp[i2].find(guess)!=cp[i2].end()){
                            cp[i2].erase(cp[i2].find(guess));
                        }
                    }
                    _solvePlugBoard(cp,nextplugboard,fms,startpos,bestFitness,bestPlugBoard);
                }
            }
            return;
        }
    }
}


EnigmaConfiguration::EnigmaConfiguration(const std::array<int, 3> fms, const pos &startpos) : startPos(startpos),
                                                                                              fms(fms) {

}

void EnigmaConfiguration::setCribIndex(int cribIndex) {
    EnigmaConfiguration::cribIndex = cribIndex;
}


std::ostream &operator<<(std::ostream &os, const EnigmaConfiguration &enigmaConfiguration) {
    os << '{' << std::endl << "\tfms: {{";
    for (int i: enigmaConfiguration.fms) {
        os << i << ',';
    }
    os << "}}" << std::endl;

    os << "\trotorStartPos: {";
    for (int i: enigmaConfiguration.startPos) {
        os << i << ',';
    }
    os << "}" << std::endl;

    os << "\tcribIndex: " << "{" << enigmaConfiguration.cribIndex << "}" << std::endl;
    os << "\tPlugBoard: " << "{" << std::endl;
    os << "\t\tBase: {" << enigmaConfiguration.plugboard << '}' << std::endl;
    os << "\t}" << std::endl;
    os << '}' << std::endl;

    return os;
}

void EnigmaConfiguration::setPlugBoard(std::string plugboard) {
    this->plugboard = plugboard;
}

void EnigmaConfiguration::setPlugboardPossibilitites(std::array<std::set<char>, 26> array1) {
    this->possible_characters=array1;
}

EnigmaConfiguration::EnigmaConfiguration() = default;


char VertexMatrix::getTurnedOffInCol(char i) {
    int start = (i - ASCII_A), end = this->size();
    for (size_t index = start; index < end; index += 26) {
        if (!(*this)[index].isPowered()) return (char) (index % 26 + ASCII_A);
    }
}

char VertexMatrix::getTurnedOffInRow(char i) {
    int start = (i - ASCII_A) * 26, end = start + 26;
    for (size_t index = start; index < end; index++) {
        if (!(*this)[index].isPowered()) return (char) (index % 26 + ASCII_A);
    }
}

int VertexMatrix::getNrPoweredOnCol(char i) {
    int nr = 0;
    int start = (i - ASCII_A), end = this->size();
    for (size_t index = start; index < end; index += 26) {
        nr += (*this)[index].getPowered();
    }
    return nr;
}

int VertexMatrix::getNrPoweredOnRow(char row_chr) const {
    int nr = 0;
    int start = (row_chr - ASCII_A) * 26, end = start + 26;
    for (size_t index = start; index < end; index++) {
        nr += (*this)[index].getPowered();
    }
    return nr;
}

void VertexMatrix::addEdge(char row_v1, char col_v1, char row_v2, char col_v2) {
    Vertex *v1 = getVertex(row_v1, col_v1);
    Vertex *v2 = getVertex(row_v2, col_v2);
    v1->addNeighbour(v2);
    v2->addNeighbour(v1);
}

Vertex *VertexMatrix::getVertex(char c1, char c2) {
    return &(*this)[(c1 - ASCII_A) * 26 + (c2 - ASCII_A)];
}

VertexMatrix::VertexMatrix() : std::array<Vertex, 676>() {
    for (int i1 = 0; i1 < 26; i1++) {
        for (int i2 = 0; i2 < 26; i2++) {
            this->at(26 * i1 + i2) = Vertex();
        }
    }
    char c1, c2;
    //    A-Y
    for (int i = 0; i < 25; i++) {
        for (int j = i + 1; j < 26; j++) {
            c1 = char(ASCII_A + i);
            c2 = char(ASCII_A + j);
            addEdge(c1, c2, c2, c1);
        }
    }
}

int Vertex::getPowered() const {
    return powered;
}

void Vertex::powerOffVertex() {
    if (isPowered()) {
        powered = 0;
        for (const auto &neighbour: neighbours) {
            neighbour->powerOffVertex();
        }
    }
}

bool Vertex::isPowered() const {
    return powered == 1;
}

void Vertex::powerVertex() {
    if (!isPowered()) {
        powered = 1;
        for (const auto &neighbour: neighbours) {
            neighbour->powerVertex();
        }
    }
}

void Vertex::addNeighbour(Vertex *v) {
    neighbours.emplace_back(v);
}
