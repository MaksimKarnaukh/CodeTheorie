//
// Created by Maksim Karnaukh on 20/10/2022.
//

#include "Adfgvx.h"


// morse code all characters
/*
const std::map<std::string, float> MORSE = {{".-", 'a'}, {"-...", 'b'}, {"-.-.", 'c'}, {"-..", 'd'}, {".", 'e'}, {"..-.", 'f'}, {"--.", 'g'}, {"....", 'h'},
                                            {"..", 'i'}, {".---", 'j'}, {"-.-", 'k'}, {".-..", 'l'}, {"--", 'm'}, {"-.", 'n'}, {"---", 'o'}, {".--.", 'p'},
                                            {"--.-", 'q'}, {".-.", 'r'}, {"...", 's'}, {"-", 't'}, {"..-", 'u'}, {"...-", 'v'}, {".--", 'w'}, {"-..-", 'x'},
                                            {"-.--", 'y'}, {"--..", 'z'}, {"-----", '0'}, {".----", '1'}, {"..---", '2'}, {"...--", '3'}, {"....-", '4'},
                                            {".....", '5'}, {"-....", '6'}, {"--...", '7'}, {"---..", '8'}, {"----.", '9'}};
*/
// morse code essential characters
const std::map<std::string, char> MORSE{{".-", 'a'}, {"-..", 'd'}, {"..-.", 'f'},
                                        {"--.", 'g'}, {"...-", 'v'}, {"-..-", 'x'}};


// order of frequency of digraphs, source: https://www.dcode.fr/bigrams
const std::map<std::string, double> DIGRAPH_EN {
    {"th", 2.9}, {"he", 2.48}, {"in", 1.87}, {"er", 1.73},
    {"an", 1.65}, {"re", 1.38}, {"es", 1.35}, {"st", 1.19},
    {"on", 1.17}, {"nd", 1.14}, {"en", 1.13}, {"at", 1.08},
    {"nt", 1.08}, {"ed", 1.04}, {"ea", 1.01}, {"to", 1},
    {"or", 0.96}, {"ti", 0.96}, {"ha", 0.94}, {"ar", 0.89},
    {"ng", 0.89}, {"is", 0.89}, {"it", 0.88}, {"te", 0.88},
    {"ou", 0.85}, {"et", 0.84}, {"of", 0.82}, {"al", 0.82},
    {"as", 0.8}, {"le", 0.74}, {"se", 0.73}, {"hi", 0.72},
    {"sa", 0.68}, {"ra", 0.64}, {"ro", 0.64}, {"ne", 0.64},
    {"ve", 0.63}, {"me", 0.62}, {"ri", 0.62}, {"so", 0.6},
    {"de", 0.59}, {"ll", 0.58}, {"ta", 0.58}, {"li", 0.57},
    {"si", 0.57}, {"el", 0.55}, {"ec", 0.52}, {"co", 0.52},
    {"no", 0.52}, {"ot", 0.51}, {"ma", 0.5}, {"di", 0.5},
    {"ic", 0.49}, {"la", 0.49}, {"ho", 0.49}, {"om", 0.48},
    {"tt", 0.48}, {"na", 0.48}, {"sh", 0.47}, {"ch", 0.46},
    {"be", 0.46}, {"ss", 0.46}, {"rt", 0.46}, {"ee", 0.45},
    {"em", 0.45}, {"ns", 0.44}, {"rs", 0.44}, {"ce", 0.43},
    {"ur", 0.42}, {"ei", 0.41}, {"ca", 0.41}, {"io", 0.41},
    {"ac", 0.4}, {"ts", 0.4}, {"da", 0.39}, {"lo", 0.39},
    {"us", 0.39}, {"wa", 0.38}, {"ni", 0.38}, {"dt", 0.38},
    {"pe", 0.38}, {"fo", 0.38}, {"ew", 0.37}, {"ut", 0.37},
    {"wi", 0.36}, {"il", 0.36}, {"eo", 0.36}, {"ly", 0.36},
    {"wh", 0.36}, {"ad", 0.35}, {"un", 0.34}, {"ow", 0.34},
    {"tr", 0.34}, {"nc", 0.33}, {"ft", 0.33}, {"do", 0.32},
    {"ge", 0.32}, {"ep", 0.32}, {"mo", 0.32}, {"we", 0.31}
};

//// trigram frequency English, source:
//const std::map<std::string, double> TRIGRAM_EN {
//    {"the", 1.81}, {"ere", 0.31}, {"hes", 0.24},
//    {"and", 0.73}, {"tio", 0.31}, {"ver", 0.24},
//    {"ing", 0.72}, {"ter", 0.30}, {"his", 0.24},
//    {"ent", 0.42}, {"est", 0.28}, {"oft", 0.22},
//    {"ion", 0.42}, {"ers", 0.28}, {"ith", 0.21},
//    {"her", 0.36}, {"ati", 0.26}, {"fth", 0.21},
//    {"for", 0.34}, {"hat", 0.26}, {"sth", 0.21},
//    {"tha", 0.33}, {"ate", 0.25}, {"oth", 0.21},
//    {"nth", 0.33}, {"all", 0.25}, {"res", 0.21},
//    {"int", 0.32}, {"eth", 0.24}, {"ont", 0.20}
// };

Adfgvx::Adfgvx(const std::string &filename) : AlgorithmDecryption(filename) {
    // sorted language letter frequencies with letter
    for (const auto &language : LETTER_FREQUENCIES) {
        std::vector<std::pair<float, char>> v;
        for (int i = 0; i < 26; i++) {
            v.emplace_back(language.second[i], 'a' + i);
        }
        languageLetterFrequencies[language.first] = v;
    }

    // sort all values in language letter frequencies
    for (auto& language : languageLetterFrequencies) {
        std::sort(language.second.begin(), language.second.end(), [](const auto &x1, const auto &x2){
            return x1.first > x2.first;
        });
    }
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

    // shows potential
    std::vector<std::vector<std::string>> potential;

    // Try with this key length
    int keyLength = 1;  // only 6 shows potential
    while (true) {
        std::cout << "Keylength: " << keyLength << "\n";
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

        // exit = false to stop
        bool exit = true;

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
            std::sort(letterFrequencies.begin(), letterFrequencies.end(),
                      [](const auto &x1, const auto &x2){
                return x1.first > x2.first;
            });

            // normalize
            double divider = 0;
            for (const auto& letterFrequency : letterFrequencies) {
                divider += (letterFrequency.first * letterFrequency.first);
            }
            divider = std::sqrt(divider);

            // compare to language letter frequency
            std::string language;
            double bestLanguageValue = 0;
            for (const auto &languageLetters : languageLetterFrequencies) {
                bool similarValues = true;
                double languageValue = 0;
                for (int i = 0; i < 10; i++) {
                    double tempValue = std::abs((double(letterFrequencies[i].first) / double(newText.size()) * 2) - (languageLetters.second[i].first));
                    if (tempValue > 0.01) {
                        similarValues = false;
                        break;
                    }
                    languageValue += tempValue;
                }
                if (similarValues) {
                    // choose best language
                    if (language.size() < 2)
                        bestLanguageValue = languageValue;
                    else if (bestLanguageValue < languageValue)
                        continue;
                    language = languageLetters.first;
                }
            }
            if (language.size() < 2) {
                continue;
            }

            // English (in this context)
            if (language != "EN") {
                continue;
            }

            // show output
            std::cout << "\nTrying ";
            for (auto i : columnIndices) {
                std::cout << i << ' ';
            }
            std::cout << std::endl << newText << "\n" << language << "\n";

            std::cout << "Percentage: ";
            for (const auto& val : letterFrequencies) {
                std::cout << val.second << ": " << double(val.first) / double(newText.size()) * 2 << ", ";
            }
            std::cout << "\n";


            // output language frequency
            std::cout << "Language: ";
            for (const auto& freq : languageLetterFrequencies[language]) {
                std::cout << freq.second << ": " << freq.first << ",  ";
            }
            std::cout << "\n";

            // substitute each pair with a letter
            std::string toSubstitute;
            int max = 26;
            for (auto& substring: substrings) {
                for (int i = 0; i < max; i++) {
                    if (substring == letterFrequencies[i].second) {
                        toSubstitute += languageLetterFrequencies[language][i].second;
                        break;
                    }
                }
            }
            std::cout << toSubstitute << "\n\n";

            // Substitution with digraph analysis (EN)
            toSubstitute = substitute(toSubstitute);
            std::cout << toSubstitute << std::endl;

            // user input
            exit = userInput(toSubstitute);


        } while (std::next_permutation(columnIndices.begin(), columnIndices.end()) && exit);

        keyLength++;
        if (keyLength > 6)
            break;
    }

    //

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


/**
 * Swap 2 characters in a string
 * @param text string to swap characters
 * @param c1 first char
 * @param c2 second char
 */
void swapCharacters(std::string &text, const char c1, const char c2) {
    for (char &c : text) {
        if (c == c1) {
            c = c2;
        }

        else if (c == c2) {
            c = c1;
        }
    }
}


/**
 * alphabetic substitution of a string
 * @param text string to substitute
 * @return substituted string
 */
std::string Adfgvx::substitute(const std::string &text) {
    // hill climbing (sort of)
    double fitness = analyseDigraphFitness(text);
    std::string bestMatch(text);
    bool changed = true;

    // try swapping characters until no changes
    while (changed) {
        changed = false;
        for (char c1 = 'a'; c1 <= 'z'; c1++) {
            for (char c2 = char(c1 + 1); c2 <= 'z'; c2++) {

                // swap character and test fitness
                std::string newText(bestMatch);
                swapCharacters(newText, c1, c2);
                double newFitness = analyseDigraphFitness(newText);
                if (newFitness > fitness) {
                    bestMatch = std::move(newText);
                    fitness = newFitness;
                    changed = true;
                }
            }

        }
    }
    return bestMatch;
}


/**
 * Given a string, analyse its digraphs and compare it to English digraph frequency
 * @param text The string
 * @return the fitness of the string
 */
double Adfgvx::analyseDigraphFitness(const std::string &text) {
    double fitness = 0;
    std::map<std::string, int> frequencies;
    int amount = 0;

    for (size_t i = 0; i < text.size(); i++) {
        if (i + 1 == text.size())
            break;
        frequencies[text.substr(i, 2)] += 1;
        amount += 1;
    }

    for (const auto& frequency : frequencies) {
        auto stringLocation = DIGRAPH_EN.find(frequency.first);
        if (stringLocation == DIGRAPH_EN.end())
            continue;

        fitness += 1 - std::abs((*stringLocation).second - (double(frequency.second) / amount * 100));
    }
    return fitness;
}


/**
 * user input 2 characters
 * @param text string to swap characters
 */
void Adfgvx::twoLetterInput(std::string &text) {
    while (true) {
        std::cout << "Enter 2 letters seperated by a space or enter 0 to cancel:\n";
        char c1, c2;
        std::cin >> c1;
        if (c1 == '0')
            break;
        std::cin >> c2;

        swapCharacters(text, c1, c2);
        std::cout << text << std::endl;
    }
}


/**
 * user input after substitution
 * @param text
 * @return
 */
bool Adfgvx::userInput(std::string &text) {
    std::string selectOption = "Please enter an option:\n\tNext permutation: 0\n\tSwap Letters: 1\n\tFinish: 2\n";
    std::cout << selectOption;
    bool wrongInput = true;
    while (wrongInput) {
        int input;
        std::cin >> input;
        switch (input) {
            case 0:
                wrongInput = false;
                break;
            case 1:
                twoLetterInput(text);
                std::cout << selectOption;
                break;
            case 2:
                return false;
            default:
                std::cout << "Please try again.\n";
                break;
        }
    }
    return true;
}