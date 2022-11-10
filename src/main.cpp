#include <iostream>
#include <string>
#include "Adfgvx.h"
#include "Enigma.h"
#include "Playfair.h"
#include "ViginerePlus.h"
#include <chrono>

using namespace std;
using namespace std::chrono;

// argv[0] holds the name of the program.
// argv[1] points to the first command line argument and argv[n] points last argument.
int main(int argc, char *argv[]) {

    std::string encr_algo;

    if(argc==2) {
        encr_algo = argv[1];
    }
    auto start = high_resolution_clock::now();

    if (encr_algo.empty()) { // all encryption algorithms
        ViginerePlus viginerePlus = ViginerePlus("../data/01-OPGAVE-vigenerePlus.txt");
    }
    else { // specific encryption algorithm

    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by main: " << duration.count() << " microseconds" << endl;

    return 0;
}
