#include <iostream>
#include <fstream>
#include <vector>

#include "hyper.hpp"

using namespace std;

auto diff_sequence(vector<int> sequence) -> vector<int>{
    bool all_zero = true;
    vector<int> differences;
    for(int i = 1; i < sequence.size(); i++){
        int d = sequence[i] - sequence[i-1];
        if(d != 0){
            all_zero = false;
        }
        differences.push_back(d);
    }
    if(all_zero){
        differences.push_back(0);
        return differences;
    }else{
        vector<int> super_differences = diff_sequence(differences); 
        differences.push_back(differences.back() + super_differences.back());
        return differences;
    }
}

int main(int argc, char **argv){
    ifstream file = hyper::open(argc, argv, "example.txt");
    string line;
    int sum = 0;
    while(getline(file, line)){
        vector<int> sequence = hyper::parse_ints(line);
        vector<int> differences = diff_sequence(sequence);
        sum += sequence.back() + differences.back();
    }
    cout << sum << endl;
}