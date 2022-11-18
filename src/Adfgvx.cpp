//
// Created by Maksim Karnaukh on 20/10/2022.
//

#include "Adfgvx.h"


Adfgvx::Adfgvx(const std::string &filename) : AlgorithmDecryption(filename) {

}

std::string Adfgvx::Solve() {
    auto text = decodeMorse(getCipherText());
    std::cout << text << std::endl;

    // Try all permutations
    unsigned short int keyLength = 1;
    bool foundKey = false;
    while (!foundKey) {
        // make columns
        std::vector<std::pair<std::string, int>> columns;
        columns.reserve(keyLength);

        // column index list
        vector<unsigned short int> columnIndices;
        columnIndices.reserve(keyLength);
        for (int columnIndex = 0; columnIndex < keyLength; columnIndex++) {
            columnIndices.push_back(columnIndex);
        }

        int columnExtra = int(text.size()) % keyLength;  // amount of columns that have an extra value
        int columnSize = std::floor(double(text.size()) / keyLength);  // minimum size of column

        // try permutations
        do {
            // testing
            std::cout << "\nTrying ";
            for (auto i : columnIndices) {
                std::cout << i << ' ';
            }
            std::cout << std::endl;

            // make columns
            columns.clear();
            int currentStringIndex = 0;
            for (const auto index : columnIndices) {
                int size = columnSize + int(index < columnExtra);
                columns.emplace_back(text.substr(currentStringIndex, size), index);
                currentStringIndex += size;
            }

            // sort the columns
            std::sort(columns.begin(), columns.end(), [](auto const& a, auto const& b) {
                return a.second < b.second;
            });

            // get string by reading each column breadth first
            auto newText = breadthFirstRead(columns, columnSize);
            std::cout << newText << "\n";
        } while (std::next_permutation(columnIndices.begin(), columnIndices.end()));

        keyLength++;
        if (keyLength > 5)
            break;
    }

    return "";
}

/**
 * Given a string in morse code, with each letter seperated by '/', returns the decoded message
 * @param text Message in morse code
 * @return The decoded message
 */
std::string Adfgvx::decodeMorse(const std::string& text) {
    std::string currentChar;
    std::string out;

    for (const char c: text) {
        if (c == '/') {
            out += MORSE.at(currentChar);
            currentChar.clear();
            continue;
        }

        currentChar += c;
    }
    return out;
}

/**
 * Returns the string by reading each column breadth first
 * @param columns The vector of columns
 * @param size The minimum size of a column
 * @return The output as string
 */
std::string Adfgvx::breadthFirstRead(const vector<std::pair<std::string, int>> &columns, int size) {
    if (columns.empty()) {
        return "";
    }

    if (columns.size() == 1) {
        return columns.front().first;
    }


    // breadth first
    int i = 0;
    string output;
    while (i < size) {
        for (const auto& column : columns) {
            output.push_back(column.first.at(i));
        }
        i++;
    }

    // check if any columns are bigger than given size
    for (const auto& column : columns) {
        if (int(column.first.size()) > size) {
            output.push_back(column.first.at(i));
        }
    }
    return output;
}
