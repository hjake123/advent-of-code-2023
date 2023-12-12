#include <iostream>
#include <fstream>
#include <vector>
#include <set>

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

/*
As a reminder, arrangement is the expanded minimum sequence with some dots added, and key is the 
sequence from the input data with ?s.
*/
auto validate_arrangement(string const &arrangement, string const &key) -> bool{
    if(arrangement.size() != key.size())
        return false;

    for(int i = 0; i < key.size(); i++){
        if(key[i] != '?' && key[i] != arrangement[i]){
            return false;
        }
    }
    return true;
}

auto build_arrangements(string const &sequence, string const &key, set<string> &arrangements, set<string> &failed_arrangements) -> bool{
    if(arrangements.count(sequence) > 0 || failed_arrangements.count(sequence) > 0){
        return arrangements.count(sequence) > 0;
    }
    if(validate_arrangement(sequence, key)){
        arrangements.emplace(sequence);
        return true;
    }
    if(sequence.size() == key.size()){
        failed_arrangements.emplace(sequence);
        return false;
    }
    
    bool success = false;
    if(key.front() == '?'){
        success = build_arrangements('.' + sequence, key, arrangements, failed_arrangements) || success;
    }
    if(key.back() == '?'){
        success = build_arrangements(sequence + ".", key, arrangements, failed_arrangements) || success;
    }
    for(int i = 1; i < sequence.size()-1; i++){
        if(sequence[i] == '.'){
            string modified = sequence;
            modified.insert(i, ".");
            success = build_arrangements(modified, key, arrangements, failed_arrangements) || success;
        }
    }
    if(!success){
        failed_arrangements.emplace(sequence);
    }
    return success;
}

int main(int argc, char **argv){
    vector<row> rows;
    ifstream file = hyper::open(argc, argv, "example.txt");
    parse_rows(file, rows);
    int sum = 0;
    int n = 0;
    for(row r : rows){
        n++;
        set<string> arrangements;
        set<string> failed_arrangements;
        build_arrangements(get_minimum_sequence(r.runs), trim_redundant_space(r.record), arrangements, failed_arrangements);
        sum += arrangements.size();
        cout << n << " : " << arrangements.size() << " with " << failed_arrangements.size() << " failed" << endl;
    }
    cout << sum;
}