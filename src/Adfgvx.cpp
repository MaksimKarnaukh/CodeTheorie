//
// Created by Maksim Karnaukh on 20/10/2022.
//

#include "Adfgvx.h"


Adfgvx::Adfgvx(const std::string &filename) : AlgorithmDecryption(filename) {

}

//void printmap(const std::map<std::string, int> &map) {
//    std::cout << map.size() << std::endl;
//    for (const auto& pair : map) {
//        std::cout << pair.first << ": " << pair.second << std::endl;
//    }
//}

std::string Adfgvx::Solve() {
    auto text = decodeMorse(getCipherText());
    std::cout << text << std::endl;

    // sorted language letter frequencies with letter
    std::map<std::string, std::vector<std::pair<float, char>>> languageLetterFrequencies;
    for (const auto &language : LETTER_FREQUENCIES) {
        std::vector<std::pair<float, char>> v;
        for (int i = 0; i < 26; i++) {
            v.emplace_back(language.second[i], 'a' + i);
        }
        languageLetterFrequencies[language.first] = v;
    }

    // sort greater function
    auto sortGreater = [](const auto &x1, const auto &x2){
        return x1.first > x2.first;
    };

    // sort all values in language letter frequencies
    for (auto& language : languageLetterFrequencies) {
        std::sort(language.second.begin(), language.second.end(), sortGreater);
    }

    // shows potential
    std::vector<std::vector<std::string>> potential;

    // Try with this key length
    int keyLength = 6;  // only 6 shows potential
    while (true) {
        // make columns
        std::vector<std::pair<std::string, int>> columns;
        columns.reserve(keyLength);

        // column index list
        std::vector<int> columnIndices;
        columnIndices.reserve(keyLength);
        for (int columnIndex = 0; columnIndex < keyLength; columnIndex++) {
            columnIndices.push_back(columnIndex);
        }

        int columnExtra = int(text.size()) % keyLength;  // amount of columns that have an extra value
        int columnSize = std::floor(double(text.size()) / keyLength);  // minimum size of column

        // try permutations
        do {
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

            // length 2 substrings and frequency
            auto substrings = splitInSubstrings(newText);
            auto frequency = twoLetterFrequency(substrings);

            // given this context, a length of 36 is unlikely
            if (frequency.size() > 35) {
                continue;
            }

            // sort frequency
            std::vector<std::pair<int, std::string>> letterFrequencies;
            letterFrequencies.reserve(35);  // cannot be more than 35
            for (const auto &substring : frequency) {
                letterFrequencies.emplace_back(substring.second, substring.first);
            }
            std::sort(letterFrequencies.begin(), letterFrequencies.end(), sortGreater);

            // remove "numbers" from frequency
//            int textSize = (int)text.size();
//            for (int i = 26; i < (int) letterFrequencies.size(); i++) {
//                textSize -= letterFrequencies[i].first;
//            }

            // normalize ???
            double divider = 0;
            for (const auto& letterFrequency : letterFrequencies) {
                divider += (letterFrequency.first * letterFrequency.first);
            }
            divider = std::sqrt(divider);

            // compare to language letter frequency
            std::string language;
            for (const auto &languageLetters : languageLetterFrequencies) {
                bool similarValues = true;
                for (int i = 0; i < 5; i++) {
                    if (std::abs((double(letterFrequencies[i].first) / divider) - languageLetters.second[i].first) > 0.05) {
                        similarValues = false;
                        break;
                    }
                }
                if (similarValues) {
                    language = languageLetters.first;
                }
            }
            if (language.size() < 2) {
                continue;
            }

            // show output
            std::cout << "\nTrying ";
            for (auto i : columnIndices) {
                std::cout << i << ' ';
            }
            std::cout << std::endl;
            std::cout << newText << "\n";
            std::cout << language << "\n";

            for (const auto& val : letterFrequencies) {
                std::cout << double(val.first) / double(newText.size()) * 200 << ", " << val.second << ", ";
            }
            std::cout << "\n";

            // some form of output
            potential.push_back(substrings);
            int max = 26;
            for (auto& substring: substrings) {
                std::string out;
                for (int i = 0; i < max; i++) {
                    if (substring == letterFrequencies[i].second) {
                        out = languageLetterFrequencies[language][i].second;
                        break;
                    }
                    else if (i == max - 1) {
                        out = '-';
                        break;
                    }
                }
                std::cout << out;
            }
            std::cout << "\n";

        } while (std::next_permutation(columnIndices.begin(), columnIndices.end()));

        std::cout << "\n" << potential.size() << std::endl;

        keyLength++;
        if (keyLength > 6)
            break;
        std::cout << keyLength << std::endl;
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
std::string Adfgvx::breadthFirstRead(const std::vector<std::pair<std::string, int>> &columns, int size) {
    if (columns.empty()) {
        return "";
    }

    if (columns.size() == 1) {
        return columns.front().first;
    }


    // breadth first
    int i = 0;
    std::string output;
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

/**
 * Counts the frequency of the substrings
 * @param substrings vector of substrings
 * @return Frequency map
 */
std::map<std::string, int> Adfgvx::twoLetterFrequency(const std::vector<std::string> &substrings) {
    std::map<std::string, int> frequency;
    for (const auto& substring : substrings) {
        frequency[substring] += 1;
    }
    return frequency;
}

/**
 * Split string into substrings with length of 2
 * @param text string to split
 * @return vector of substrings
 */
std::vector<std::string> Adfgvx::splitInSubstrings(const std::string &text) {
    std::vector<std::string> substrings;
    for (int i = 0; i < int(text.size()); i += 2) {
        substrings.push_back(text.substr(i, 2));
    }
    return substrings;
}
