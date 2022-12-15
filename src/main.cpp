#include <iostream>
#include <string>
#include "Adfgvx.h"
#include "Enigma.h"
#include "Playfair.h"
#include "ViginerePlus.h"
#include <chrono>
#include <memory>
#include <complex>
#include <algorithm>

using namespace std;
using namespace std::chrono;

// argv[0] holds the name of the program.
// argv[1] points to the first command line argument and argv[n] points last argument.
int main(int argc, char *argv[]) {
    std::string encr_algo;
    if(argc>=2) {
        encr_algo = argv[1];
    }
    // temporary fix when no arguments are given
    else {
        std::cout << "Please run with one of the following arguments:\nVP\tPF\tAD\tEN\n";
        return 0;
    }

    auto start = high_resolution_clock::now();
    std::shared_ptr<AlgorithmDecryption> algorithmDecryption;

    if (encr_algo.empty()) { // all encryption algorithms
        std::cout << "Please enter an encryption: VP, PF, AD, EN" << std::endl;
        return 1;
//        algorithmDecryption = std::make_shared<ViginerePlus>(ViginerePlus("../data/01-OPGAVE-vigenerePlus.txt"));
    }
    else if ( encr_algo == "VP"){ // specific encryption algorithm
        algorithmDecryption = std::make_shared<ViginerePlus>(ViginerePlus("../data/01-OPGAVE-vigenerePlus.txt"));
    }
    else if ( encr_algo == "PF"){ // specific encryption algorithm
        algorithmDecryption = std::make_shared<Playfair>(Playfair("../data/02-OPGAVE-playfair.txt"));
    }
    else if (encr_algo == "AD") {
        algorithmDecryption = std::make_shared<Adfgvx>(Adfgvx("../data/03-OPGAVE-adfgvx.txt"));
    }
    else if (encr_algo == "EN") {
        algorithmDecryption = std::make_shared<Enigma>(Enigma("../data/04-OPGAVE-enigma.txt"));
//        algorithmDecryption = std::make_shared<Enigma>(Enigma("../data/EngimaTest.txt"));
    }
    else {
        std::cout << "Please run with one of the following arguments:\nVP\tPF\tAD\tEN\n";
        return 0;
    }
    else { // all encryption algorithms
        std::cout << "Please enter a supported encryption: VP, PF, AD, EN" << std::endl;
        return 1;
//        algorithmDecryption = std::make_shared<ViginerePlus>(ViginerePlus("../data/01-OPGAVE-vigenerePlus.txt"));
    }
    std::string out = algorithmDecryption->Solve();
    std::cout << out << std::endl;
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);
    cout << "Time taken by main: " << duration.count() << " seconds" << endl;
    return 0;
}
