//
// Created by Maksim Karnaukh on 20/10/2022.
//

#include "ViginerePlus.h"

ViginerePlus::ViginerePlus(const std::string &filename) : AlgorithmDecryption(filename) {

}

/**
 * Solvest the Viginere ciphertext by trying all columntransformations and selecting the best ones using a set for ordering.
 * Writes the best column transposes to
 * @return the resulting string ViginerePlus_ColumnTransposition.txt
 */
std::string ViginerePlus::Solve() {
//    Get a vector with most likely solutions of the column transpostion
    std::set<SingleColumnTransposition, std::greater<>> columnTranspositions = this->SolveColumnTranspostion(2, 9, 3);

    std::string bestKey{}, tempKey{};
    double tempFittness{};
    std::numeric_limits<double>::max();
    std::pair<std::string, double> temp{};
    std::map<double, std::map<std::string, SingleColumnTransposition>> results{};

    for (auto &columnTranspo: columnTranspositions) {
        ViginerePlus::findBestKey(columnTranspo.getResult(), tempKey, tempFittness);
        std::cout << "Key: " << tempKey << " Fitness: " << tempFittness << std::endl;
        if (results.find(tempFittness) == results.end()) {
            results[tempFittness] = {{tempKey, columnTranspo}};
        } else {
            results[tempFittness].insert({tempKey, columnTranspo});
        }
    }
    std::string input, plaintext;
    for (const auto &[fitness2, result]: results)
        for (const auto &[key, columnTranspo]: result) {
            plaintext = decrypt(columnTranspo.getResult(), key);
            std::cout << "Text:" << std::endl << plaintext << std::endl << std::endl;
            std::cout << "Fitness: " << fitness2 << " Key: " << key << std::endl << std::endl;
            std::cout << "Is the following text readable? y/n";
            std::cin >> input;
            if (input.find('y') != std::string::npos) {
                return decrypt(columnTranspo.getResult(), key);
            }
        }


    return "Error";
}

/**
 *
 * @param min_keylength minimum length of the key
 * @param max_keylength maximum length of the key (including)
 * @param nr_to_keep nr of column transpostion to keep and return
 * @return a set with the most prominent column transposition first
 */
std::set<SingleColumnTransposition, std::greater<>>
ViginerePlus::SolveColumnTranspostion(int min_keylength, int max_keylength, size_t nr_to_keep) {
    //    For timing and feedback purposes
    auto start = std::chrono::high_resolution_clock::now();
    unsigned int counter = 0;
//    Will hold the time difference
    long long diff;
//    Will hold the current permutation
    std::vector<int> v;
//    Set with the nr_to_keep-best column-transposition results, ordering with greatest first (check SingleColumnTransposition::operator> for more info about selection)
    std::set<SingleColumnTransposition, std::greater<>> transposed;
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
                std::cout << "\t|Counter: " << std::to_string(counter) << "\t |Time: "
                          << diff
                          << "\t|Iterations/Second: " << counter / diff << std::endl;
            }
//            if we havent saved n transpositions yet or if the new one is better than the worst-saved one
            if (transposed.size() < nr_to_keep) {
//                insert it
                transposed.insert(temp);
            } else if (temp > *transposed.rbegin()) {
                transposed.erase(--transposed.end());
                transposed.insert(temp);
            }
        }
//        Output stats of the key with length keysize
        std::cout << "Keylength: " << std::to_string(keysize) << " Counter: " << std::to_string(counter) << " Time: "
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
    } else std::cout << "Unable to open file";
    return transposed;
}

void ViginerePlus::findBestKey(const std::string &basicString, std::string &return_key, double &return_fitness) {
    std::string substring{}, key{};
    char best_char{};
    double best_fitness, fitness;
    std::map<std::string, double> keys;
    std::vector<double> frequencies;
    return_fitness = std::numeric_limits<double>::max();
    for (int keyLength = 2; keyLength <= 10; keyLength++) {
        key = std::string(keyLength, 'x');
        for (int start_index = 0; start_index <= keyLength; start_index++) {
            substring = {};
            best_fitness = std::numeric_limits<double>::max();
            for (std::size_t index = start_index, stringSize = basicString.size();
                 index < stringSize; index += keyLength) {
                substring += basicString[index];
            }
            getAlphabetFrequencies(substring, frequencies);

            for (std::size_t i = 0, frequenciesSize = frequencies.size(); i < frequenciesSize; i++) {
                fitness = AlgorithmDecryption::compareLanguageFrequencies(frequencies);
                if (fitness < best_fitness) {
                    best_char = char(i + ASCII_A);
                    best_fitness = fitness;
                }
                std::rotate(frequencies.begin(), frequencies.begin() + 1, frequencies.end());
            }

            key[start_index] = best_char;
        }
        getAlphabetFrequencies(decrypt(basicString, key), frequencies);
        fitness = AlgorithmDecryption::compareLanguageFrequencies(frequencies);
        if (fitness < return_fitness) {
            return_fitness = fitness;
            return_key = key;
        }

    }

}

std::string ViginerePlus::decrypt(const std::string &ciphertext, const std::string &key) {
    std::string plaintext(ciphertext.length(), ' ');
    std::size_t keyLength = key.length();
    int keyChar;
    for (std::size_t start_index = 0; start_index < keyLength; start_index++) {
        keyChar = key[start_index];
        for (std::size_t index = start_index, ciphertextSize = ciphertext.size();
             index < ciphertextSize; index += keyLength) {
            plaintext[index] = (char) (( ciphertext[index] - keyChar + 26) % 26 + ASCII_A);
        }
    }
    return plaintext;
}

/**
 *
 * @param order the order of the transpostion as std::vector, with index being the original column and the value the resulting column
 * @param ciphertext the ciphertext of which to start
 */
SingleColumnTransposition::SingleColumnTransposition(const std::vector<int> &order, const std::string &ciphertext) {
//    save the order for possible output later
    this->order = order;
//    make every transposition differentiable
    static int _id = 0;
    id = _id++;

//    construct the result (will be saved in this->result)
    constructResult(ciphertext);

//    extract information about substring frequention with max length of maxlength (how often a repeated squence occures in the result)
    int maxlength = 3;
    this->m = AlgorithmDecryption::subStringFrequention(this->result, maxlength);
}

bool SingleColumnTransposition::operator>(const SingleColumnTransposition &other) const {
    // Store the reverse iterator to the last element of the map in a local variable
    auto it = this->m.rbegin();

//    Compare the maximum frequencies of the two objects
    if (it->first == other.m.rbegin()->first) {
//       Compare the number of strings in the most frequent entry
        if (it->second.size() == other.m.rbegin()->second.size()) {
//            differentiate on id, higher is better (although both are equally good, otherwise we get undefined behaviour)
            return this->id > other.id;
        }
//        the one with more entries is better
        return it->second.size() > other.m.rbegin()->second.size();
    }
//    the one with the larger maximum frequencies is better
    return it->first > other.m.rbegin()->first;
}

SingleColumnTransposition::SingleColumnTransposition() {
    static int _id = 0;
    id = _id++;
}

bool SingleColumnTransposition::operator==(const SingleColumnTransposition &other) const {
    return this->result == other.result;
}

std::ostream &operator<<(std::ostream &os, const SingleColumnTransposition &singleColumnTransposition) {
    for (const auto &el: singleColumnTransposition.order) {
        os << std::to_string(el) + "; ";
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

void SingleColumnTransposition::fillEmpties(std::string &basicString) {
//    key length
    size_t keylength = getKeyLength();
//    number of empties = length of the key - the rest of the string/keylength
    int nr_empties = (int) ((keylength - basicString.length() % keylength) % keylength);
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

std::size_t SingleColumnTransposition::getKeyLength() {
    return this->order.size();
}

/**
 * calculates and returns the amount of characters per column using getKeyLength()
 * @param stringlength the length of the string
 * @return
 */
int SingleColumnTransposition::getCharPerCol(size_t stringlength) {
//    we need keylength
    std::size_t keylength = getKeyLength();
//    ceil(length of string/length of key)
    return std::ceil((double) stringlength / (double) keylength);
}

void SingleColumnTransposition::constructResult(std::string ciphertext) {
//    refill the empty spaces of the string with spaces where they are supposed to be after transformation
    this->fillEmpties(ciphertext);
//    start with an empty result of cipherlength.length
    this->result = std::string(ciphertext.length(), ' ');
//    reserve memory for place, orgiginal_index; initialize keyLength and characters per column
    int place, original_index, char_per_col = getCharPerCol(ciphertext.length());
    size_t keyLength = this->getKeyLength();
//    reserve memory for the current substring
    std::string substring;
//    col_nr = the index of the original column
    for (size_t col_nr = 0; col_nr < keyLength; col_nr++) {
//        place = the index of the transposed column
        place = this->order[col_nr];
//        fetch the string of char_per_col characters starting from the transposed-column which starts at it's index * characters_per_col
//        characters belonging to one column
        substring = ciphertext.substr(place * char_per_col, char_per_col);
//        loop over the characters from the substring
        for (int char_index = 0; char_index < char_per_col; char_index++) {
//            original index = the column number + the index of the character in the substring * the keyLength
            original_index = (int) col_nr + char_index * (int) this->order.size();
//            replace the character in the result with the one in the substring
            this->result[original_index] = substring[char_index];
        }
    }
//    remove the empty characters again
    removeEmpties(this->result);
}


void SingleColumnTransposition::removeEmpties(std::string &basicString) {
    int len = 0;
//    start at the end
    for (size_t index = basicString.length() - 1; basicString[index] == ' '; index--) {
//        as long as we find spaces
        len += 1;
    }
//    erase all len spaces at the end of the string
    basicString.erase(basicString.length() - len, len);
}

SingleColumnTransposition::~SingleColumnTransposition() = default;

std::string SingleColumnTransposition::Solve() {
    return this->result;
}

const std::string &SingleColumnTransposition::getResult() const {
    return result;
}
