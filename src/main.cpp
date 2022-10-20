#include <iostream>
#include "Adfgvx.h"
#include "Enigma.h"
#include "Playfair.h"
#include "ViginerePlus.h"

// argv[0] holds the name of the program.
// argv[1] points to the first command line argument and argv[n] points last argument.
int main(int argc, char *argv[]) {

    if(argc==1) {
        printf("\nNo Extra Command Line Argument Passed");
    }
    if(argc==2) {
        const std::string encr_algo = argv[1];
        std::cout << encr_algo << std::endl;
    }

    return 0;
}
