//
// Created by Maksim Karnaukh on 20/10/2022.
//

#include "ViginerePlus.h"
#include <algorithm>
#include <cassert>
#include <chrono>
#include <fstream>      // std::ofstream
#include <cmath>

ViginerePlus::ViginerePlus(const std::string &filename) : AlgorithmDecryption(filename) {

}

/**
 * Solvest the Viginere ciphertext by trying all columntransformations and selecting the best ones using a set for ordering.
 * Writes the best column transposes to
 * @return the resulting string ViginerePlus_ColumnTransposition.txt
 */
std::string ViginerePlus::Solve() {

//    For timing and feedback purposes
    auto start = std::chrono::high_resolution_clock::now();
    unsigned int counter = 0;
//    Keysizes [min_keylength, max_keylength] (inclusive)
    int min_keylength = 2, max_keylength = 10;
//    Will hold the time difference
    int diff;
//    Will hold the current permutation
    std::vector<int> v;
//    Set with the n-best column-transposition results, ordering with greatest first (check SingleColumnTransposition::operator> for more info about selection)
    int n = 5;
    std::set<SingleColumnTransposition, std::greater<SingleColumnTransposition>> transposed = {};
//    The current transposition
    SingleColumnTransposition temp;
//        Loop over all possible keylengths
    for (int keysize = min_keylength; keysize <= max_keylength; keysize++) {
//        initialize the vector {0,1,2,3, ..., keysize}
        vectorWithRange(v, 0, keysize);
//        take the next permutation, if there is one
        while (std::next_permutation(v.begin(), v.end())) {
//            initialize the column transposition and analyze the result
            temp = SingleColumnTransposition(v, this->getCipherText());
//            Output
            counter++;
            if (counter % 10000 == 0) {
//                time diff since start
                diff = std::chrono::duration_cast<std::chrono::seconds>(
                        std::chrono::high_resolution_clock::now() - start).count();
//                Output counter, time and interations per second
                std::cout << "\t|Counter: " << to_string(counter) << "\t |Time: "
                          << diff
                          << "\t|Iterations/Second: " << counter / diff << std::endl;
            }
//            if we havent saved n transpositions yet or if the new one is better than the worst-saved one
            if (transposed.size() < n || temp > *transposed.rbegin()) {
//                insert it
                transposed.insert(temp);
            }
        }
//        Output stats of the key with length keysize
        std::cout << "Keylength: " << to_string(keysize) << " Counter: " << to_string(counter) << " Time: "
                  << std::chrono::duration_cast<std::chrono::seconds>(
                          std::chrono::high_resolution_clock::now() - start).count() << std::endl;
    }
//    save the resulting column transpositions to a file
    std::fstream ofs;
    ofs.open("ViginerePlus_ColumnTransposition.txt", std::ofstream::out | std::ofstream::trunc);
    if (ofs.is_open()) {
        for (const auto &item: transposed) {
            ofs << item << std::endl;
        }
        ofs.close();
    } else cout << "Unable to open file";
    return std::string();
}

/**
 *
 * @param order the order of the transpostion as std::vector, with index being the original column and the value the resulting column
 * @param ciphertext the ciphertext of which to start
 */
SingleColumnTransposition::SingleColumnTransposition(const vector<int> &order, const string &ciphertext) {
//    save the order for possible output later
    this->order = order;
//    make every transposition differentiable
    static int _id = 0;
    id = _id++;

//    construct the result (will be saved in this->result)
    constructResult(ciphertext);

//    extract information about substring frequention with max length of maxlength (how often a repeated squence occures in the result)
    int maxlength = 4;
    this->m = AlgorithmDecryption::subStringFrequention(this->result, maxlength);
}

bool SingleColumnTransposition::operator>(const SingleColumnTransposition &other) const {
//    if they have the same amount of maximum frequencies
    if (this->m.rbegin()->first == other.m.rbegin()->first) {
//       if they also have the same amount of strings in the most frequent entry
        if (this->m.rbegin()->second.size() == other.m.rbegin()->second.size()) {
//            differentiate on id, higher is better (although both are equally good, otherwise we get undefined behaviour)
            return this->id > other.id;
        }
//        the one with more entries is better
        return this->m.rbegin()->second.size() > other.m.rbegin()->second.size();
    }
//    the one with the largers maximum frequencies is better
    return this->m.rbegin()->first > other.m.rbegin()->first;
}

SingleColumnTransposition::SingleColumnTransposition() {

}

bool SingleColumnTransposition::operator==(const SingleColumnTransposition &other) const {
    return this->result == other.result;
}

ostream &operator<<(ostream &os, const SingleColumnTransposition &singleColumnTransposition) {
    for (const auto &el: singleColumnTransposition.order) {
        os << to_string(el) + "; ";
    }
    os << std::endl;
    os << "Result = " + singleColumnTransposition.result;
    os << std::endl;
    for (const auto &[key, elem]: singleColumnTransposition.m) {
        os << key << std::endl;
        for (const auto &str: elem) {
            os << "\t" << str << std::endl;
        }
    }
    return os;
}

void SingleColumnTransposition::fillEmpties(string &basicString) {
//    key length
    int keylength = getKeyLength();
//    number of empties = length of the key - the rest of the string/keylength
    int nr_empties = (keylength - basicString.length() % keylength)%keylength;
//    the columns containing the empty elements, starting from last to last-nr_empties (original columns)
    std::set<int> empties(this->order.rbegin(), this->order.rbegin() + nr_empties);
    int char_per_col = getCharPerCol(basicString.length());
//    Loop over the transposed columns which contain empties
    for (const auto &empty: empties) {
//        insert an empty in the last character of the column-substring
        int index = char_per_col * (empty + 1);
        basicString.insert(index - 1, " ");
    }
}

int SingleColumnTransposition::getKeyLength() {
    return this->order.size();
}

/**
 * calculates and returns the amount of characters per column using getKeyLength()
 * @param stringlength the length of the string
 * @return
 */
int SingleColumnTransposition::getCharPerCol(int stringlength) {
//    we need keylength
    int keylength = getKeyLength();
//    ceil(length of string/length of key)
    return std::ceil((float) stringlength / keylength);
}

void SingleColumnTransposition::constructResult(std::string ciphertext) {
//    refill the empty spaces of the string with spaces where they are supposed to be after transformation
    this->fillEmpties(ciphertext);
//    start with an empty result of cipherlength.length
    this->result = std::string(ciphertext.length(), ' ');
//    reserve memory for place, orgiginal_index; initialize keyLength and characters per column
    int place, original_index, keyLength = this->getKeyLength(), char_per_col = getCharPerCol(ciphertext.length());
//    reserve memory for the current substring
    std::string substring;
//    col_nr = the index of the original column
    for (int col_nr = 0; col_nr < keyLength; col_nr++) {
//        place = the index of the transposed column
        place = this->order[col_nr];
//        fetch the string of char_per_col characters starting from the transposed-column which starts at it's index * characters_per_col
//        characters belonging to one column
        substring = ciphertext.substr(place * char_per_col, char_per_col);
//        loop over the characters from the substring
        for (int char_index = 0; char_index < char_per_col; char_index++) {
//            original index = the column number + the index of the character in the substring * the keyLength
            original_index = col_nr + char_index * this->order.size();
//            replace the character in the result with the one in the substring
            this->result[original_index] = substring[char_index];
        }
    }
//    remove the empty characters again
    this->removeEmpties(this->result);
}


void SingleColumnTransposition::removeEmpties(string &basicString) {
    int len = 0;
//    start at the end
    for (int index = basicString.length() - 1; basicString[index] == ' '; index--) {
//        as long as we find spaces
        len += 1;
    }
//    erase all len spaces at the end of the string
    basicString.erase(basicString.length() - len, len);
}

SingleColumnTransposition::~SingleColumnTransposition() {

}

std::string SingleColumnTransposition::Solve() {
    return this->result;
}
