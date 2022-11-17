//
// Created by Maksim Karnaukh on 20/10/2022.
//

#ifndef CODETHEORIE_VIGINEREPLUS_H
#define CODETHEORIE_VIGINEREPLUS_H

#include "AlgorithmDecryption.h"

class ViginerePlus : public AlgorithmDecryption {


public:
    ViginerePlus(const std::string& filename);
    std::string Solve() override;

};

class SingleColumnTransposition : AlgorithmDecryption {
    std::string result, ciphertext;
    std::vector<int> order;
    map<int, set<std::string>> m;
    int id;
public:
    SingleColumnTransposition(const std::vector<int>& order,const std::string& ciphertext);
    bool operator> (const SingleColumnTransposition& other) const;
    bool operator== (const SingleColumnTransposition& other ) const;
    SingleColumnTransposition();
    friend ostream& operator<<(ostream& os, const SingleColumnTransposition& singleColumnTransposition);

    void fillEmpties(string &basicString);
    int getKeyLength();
    int getCharPerCol(int stringlength);

    void constructResult(std::string ciphertext);

    virtual ~SingleColumnTransposition();
    std::string Solve() override;
    void removeEmpties(string &basicString);
};



#endif //CODETHEORIE_VIGINEREPLUS_H
