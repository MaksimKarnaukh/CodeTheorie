//
// Created by Maksim Karnaukh on 20/10/2022.
//

#ifndef CODETHEORIE_PLAYFAIR_H
#define CODETHEORIE_PLAYFAIR_H

#include "AlgorithmDecryption.h"
#include <cmath>
#include <iomanip>
using namespace std;
class Playfair : public AlgorithmDecryption {

public:

    mt19937 random_engine;

    /**
     * Playfair constructor function
     * @param filename: text file containing the ciphertext encrypted with playfair.
     */
    Playfair(const std::string& filename);

    /**
     * main function

     * Simulated Annealing Algorithm :

        1. Generate a random key, called the 'parent', decipher the ciphertext
        using this key.
        2. Rate the fitness of the deciphered text, store the result.
        3. for(TEMP = 10; TEMP >= 0; TEMP = TEMP - STEP)
                for (count = 50,000; count>0; count--)
                    Change the parent key slightly (e.g. swap two characters in the
                        key at random) to get child key,
                    Measure the fitness of the deciphered text using the child key
                    set dF = (fitness of child - fitness of parent)
                    If dF > 0 (fitness of child is higher than parent key),
                        set parent = child
                    If dF < 0 (fitness of child is worse than parent),
                        set parent = child with probability e^(dF/T).

     * @return: best key
     */
    std::string Solve() override;

    /**
     * Modulo function that performs correctly for negative numbers. This needed to be self-implemented since the
     * standard C++ mod() function doesn't give the correct answer when using negative numbers.
     * @param a: first number
     * @param b: second number (modulus)
     * @return remainder (a mod b)
     */
    int mod(int a, int b);

    /**
     *  Changes the key with the following actions and corresponding frequencies:

        Swap single letters (90%)
        Swap random rows (2%)
        Swap random columns (2%)
        Flip all rows (2%) (outside becomes inside and vice versa)
        Flip all columns (2%) (outside becomes inside and vice versa)
        Reverse the whole key (2%) (we'll use the reverse() function)

     * @param key : string consisting of 25 different letters.
     * @return : modified key
     */
    string modifyKey(const string& key);

    /**
     * See Playfair::modifyKey(string) function description.
     */
    string swapTwoLetters(const string& key);
    string swapTwoRows(const string& key);
    string swapTwoColumns(const string& key);
    string flipAllRows(const string& key);
    string flipAllColumns(const string& key);


    /**
     * Decrypts the ciphertext with the given key according to the playfair algorithm.
     * @param key : string consisting of 25 different letters.
     * @param ciphertext : ciphertext
     * @return : plaintext (decrypted text with the key)
     */
    string decipher(const string& key, const string& ciphertext);



};


#endif //CODETHEORIE_PLAYFAIR_H
