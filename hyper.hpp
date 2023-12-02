#pragma once
#include <iostream>
#include <fstream>

auto hyper_open(int const argc, char const * const * const argv, std::string fallback_filename) -> std::ifstream{
    std::string filename = argc == 1 ? fallback_filename : argv[1];
    std::ifstream input_file = std::ifstream(filename);
    if(!input_file.is_open()){
        std::cerr << "Couldn't find input file " << filename;
        exit(1);
    }
    return input_file;
}
