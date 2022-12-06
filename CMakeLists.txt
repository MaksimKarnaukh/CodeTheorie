cmake_minimum_required(VERSION 3.17)
project(CodeTheorie)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_STANDARD 17)

add_executable(CodeTheorie src/main.cpp src/ViginerePlus.cpp src/ViginerePlus.h src/Playfair.cpp src/Playfair.h src/Adfgvx.cpp src/Adfgvx.h src/Enigma.cpp src/Enigma.h src/AlgorithmDecryption.cpp src/AlgorithmDecryption.h)