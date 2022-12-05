//
// Created by Maksim Karnaukh on 20/10/2022.
//

#ifndef CODETHEORIE_VIGINEREPLUS_H
#define CODETHEORIE_VIGINEREPLUS_H

#include <memory>
#include "AlgorithmDecryption.h"
#include <algorithm>
#include <chrono>
#include <fstream>      // std::ofstream
#include <cmath>
#include <memory>

class SingleColumnTransposition : AlgorithmDecryption {
    std::string result;
    std::vector<int> order;
    std::map<int, std::set<std::string>> m;
    int id;
public:
    SingleColumnTransposition(const std::vector<int> &order, const std::string &ciphertext);

    bool operator>(const SingleColumnTransposition &other) const;

    bool operator==(const SingleColumnTransposition &other) const;

    SingleColumnTransposition();

    friend std::ostream &operator<<(std::ostream &os, const SingleColumnTransposition &singleColumnTransposition);

    void fillEmpties(std::string &basicString);

    std::size_t getKeyLength();

    int getCharPerCol(size_t stringlength);

    void constructResult(std::string ciphertext);

    ~SingleColumnTransposition() override;

    std::string Solve() override;

    static void removeEmpties(std::string &basicString);

    [[nodiscard]] const std::string &getResult() const;
};

class ViginerePlus : public AlgorithmDecryption {
public:
    explicit ViginerePlus(const std::string &filename);

    std::string Solve() override;

    std::set<SingleColumnTransposition, std::greater<>>
    SolveColumnTranspostion(int min_keylength, int max_keylength, size_t nr_to_keep);

    static void findBestKey(const std::string &basicString, std::string &return_key, double &return_fitness);

    static std::string decrypt(const std::string &ciphertext, const std::string &key);
};


#endif //CODETHEORIE_VIGINEREPLUS_H
