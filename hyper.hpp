#pragma once
#include <iostream>
#include <fstream>
#include <vector>

namespace hyper{
auto open(int const argc, char const * const * const argv, std::string const &fallback_filename) -> std::ifstream{
    std::string filename = argc == 1 ? fallback_filename : argv[1];
    std::ifstream input_file = std::ifstream(filename);
    if(!input_file.is_open()){
        std::cerr << "Couldn't find input file " << filename;
        exit(1);
    }
    return input_file;
}

/*
Split a string into a vector of strings seperated by spaces.
This DOES reallocate memory for everything in the original string, but like these are usually
less then a kilobyte so it seems alright.
*/
auto split_by_space(std::string const &in) -> std::vector<std::string>{
    std::vector<std::string> tokens;
    std::string buffer;
    for(char c : in){
        if(c == ' ' && buffer.size() > 0){
            tokens.push_back(buffer);
            buffer = "";
        }else if(c != ' '){
            buffer.push_back(c);
        }
    }
    if(buffer.size() > 0){
        tokens.push_back(buffer);
    }
    return tokens;
}

/*
Parse a vector of strings into a vector of ints, leaving out any that start with a non-digit character.
*/
auto parse_ints(std::vector<std::string> tokens) -> std::vector<int>{
    std::vector<int> numbers;
    for(std::string token : tokens){
        if(token.size() != 0 && (isdigit(token[0]) || token[0] == '-' && isdigit(token[1]))){
            numbers.push_back(stoi(token));
        }
    }
    return numbers;
}

/*
Parse space-seperated integers from a string.
*/
auto parse_ints(std::string line) -> std::vector<int>{
    return parse_ints(split_by_space(line));
}

/*
Parse a vector of strings into a vector of long longs, leaving out any that start with a non-digit character.
*/
auto parse_long_longs(std::vector<std::string> tokens) -> std::vector<long long>{
    std::vector<long long> numbers;
    for(std::string token : tokens){
        if(token.size() != 0 && isdigit(token[0])){
            numbers.push_back(stoll(token));
        }
    }
    return numbers;
}

/*
Parse space-seperated long longs from a string.
*/
auto parse_long_longs(std::string line) -> std::vector<long long>{
    return parse_long_longs(split_by_space(line));
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

void debug_print(std::vector<int> ints){
    for(int i : ints){
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

auto sum(std::vector<int> ints) -> int{
    int sum = 0;
    for (int i : ints){
        sum += i;
    }
    return sum;
}
}
