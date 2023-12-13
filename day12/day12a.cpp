#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <cmath>
#include <algorithm>

#include "hyper.hpp"

using namespace std;

struct row{
    string record;
    vector<int> runs;
};

void debug_print(vector<row> rows){
    for(row r : rows){
        cout << r.record << " -- ";
        hyper::debug_print(r.runs);
    }
}

void parse_rows(ifstream &file, vector<row> &rows){
    string line;
    while(getline(file, line)){
        vector<string> halves = hyper::split_by_space(line);
        vector<int> numbers = hyper::parse_ints(hyper::split(halves[1], ','));
        rows.push_back(row{halves[0], numbers});
    }
}

auto n_pounds(int n) -> string{
    string sequence = "";
    for(int i = 0; i < n; i++){
        sequence.push_back('#');
    }
    return sequence;
}

auto get_minimum_sequence(vector<int> const &nums) -> string{
    string sequence;
    for(int n : nums){
        sequence.append(n_pounds(n));
        sequence.push_back('.');
    }
    sequence.pop_back();
    return sequence;
}

auto trim_redundant_space(string const &record) -> string{
    string trimmed;
    bool last_was_dot;
    for(char c : record){
        if(c == '.'){
            if(!last_was_dot){
                last_was_dot = true;
                trimmed.push_back(c);
            }
        }else{
            last_was_dot = false;
            trimmed.push_back(c);
        }
    }
    if(trimmed.front() == '.'){
        trimmed = trimmed.substr(1);
    }
    if(trimmed.back() == '.'){
        trimmed.pop_back();
    }
    return trimmed;
}

auto validate(string const &sequence, string const &key) -> bool{
    if(sequence.size() != key.size())
        return false;

    for(int i = 0; i < key.size(); i++){
        if(key[i] != '?' && key[i] != sequence[i]){
            return false;
        }
    }
    return true;
}

int main(int argc, char **argv){
    vector<row> rows;
    ifstream file = hyper::open(argc, argv, "example.txt");
    parse_rows(file, rows);
    debug_print(rows);
    int sum = 0;

    for(row r : rows){
        
        
    }
    cout << sum;
}