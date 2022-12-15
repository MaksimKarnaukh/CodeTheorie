# CodeTheorie

Push naar Master

Base class: AlgorithmDecryption 

Subclasses that inherit from AlgorithmDecryption: \
Adfgvx \
Enigma \
Playfair \
VigenerePlus

Run file: main.cpp

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-this-project">About This Project</a>
    </li>
    <li>
      <a href="#instructions">Instructions</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#compilation">Compilation</a></li>
        <li><a href="#usage">Usage</a></li>
      </ul>
    </li>
  </ol>
</details>

<!-- ABOUT THIS PROJECT -->
## About This Project
Decrypting messages using the following algorithms:
* ViginèrePlus
* Playfair
* ADFGVX
* Enigma

This project is written entirely in c++. Instructions for usage can be found below.

<!-- INSTRUCTIONS -->
## Instructions

Instructions on how to compile and run.

### Prerequisites

* [CMake](https://cmake.org/install/)

_MacOS users can use [Homebrew](https://brew.sh/) to install CMake_
```sh
brew install cmake
```

### Compilation

1.  Create build directory
    ```sh
    mkdir build && cd build
    ```
    
2.  CMake and Make
    ```sh
    cmake .. && make
    ```
    
### Usage
To run a specific algorithm, use one of the following:
* ViginèrePlus
    ```sh
    ./CodeTheorie VP
    ```
* Playfair
    ```sh
    ./CodeTheorie PF
    ```
* ADFGVX
    ```sh
    ./CodeTheorie AD
    ```
* Enigma
    ```sh
    ./CodeTheorie EN
    ```
  
### Notes
Some algorithms will require user input. Entering wrong inputs may break the program.