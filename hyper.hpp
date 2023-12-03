#pragma once
#include <iostream>
#include <fstream>
#include <vector>

auto hyper_open(int const argc, char const * const * const argv, std::string const &fallback_filename) -> std::ifstream{
    std::string filename = argc == 1 ? fallback_filename : argv[1];
    std::ifstream input_file = std::ifstream(filename);
    if(!input_file.is_open()){
        std::cerr << "Couldn't find input file " << filename;
        exit(1);
    }
    return input_file;
}

void debug_print(std::vector<std::vector<char>> grid){
    for(std::vector<char> v : grid){
        for(char c : v){
            std::cout << c;
        }
        std::cout << "\n";
    }
    std::cout << std::flush;
}

void debug_print(std::vector<std::vector<bool>> grid){
    for(std::vector<bool> v : grid){
        for(bool b : v){
            std::cout << b;
        }
        std::cout << "\n";
    }
    std::cout << std::flush;
}

void debug_print(std::vector<std::vector<int>> grid){
    for(std::vector<int> v : grid){
        for(int i : v){
            std::cout << i << " ";
        }
        std::cout << "\n";
    }
    std::cout << std::flush;
}

auto sum(std::vector<int> ints) -> int{
    int sum = 0;
    for (int i : ints){
        sum += i;
    }
    return sum;
}
