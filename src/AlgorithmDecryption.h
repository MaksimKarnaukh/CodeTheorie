//
// Created by Maksim Karnaukh on 20/10/2022.
//

#ifndef CODETHEORIE_ALGORITHMDECRYPTION_H
#define CODETHEORIE_ALGORITHMDECRYPTION_H

#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <streambuf>
using namespace std;

/**
 * AlgorithmDecryption: Base class for all the decryption classes
 */
class AlgorithmDecryption {

    // to store the ciphertext from the file in
    string cipherText;

public:

    AlgorithmDecryption();

    const string &getCipherText() const;

    AlgorithmDecryption(const std::string& filename);
    virtual ~AlgorithmDecryption() = default;
    void vectorWithRange(std::vector<int> &v, int start, int end);
    virtual std::string Solve() = 0;

    static std::map<int, std::set<std::string>> subStringFrequention(const string &text, int max_length);

    static void printMap(map<int, set<std::string>> m);
};


#endif //CODETHEORIE_ALGORITHMDECRYPTION_H
