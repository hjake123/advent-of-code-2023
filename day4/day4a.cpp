#include <iostream>
#include <fstream>
#include <vector>
#include <set>

#include "hyper.hpp"

using namespace std;

int main(int argc, char **argv){
    auto infile = hyper::open(argc, argv, "example.txt");
    int score = 0;
    while(!infile.eof()){
        string line;
        getline(infile, line);
        vector<string> words = hyper::split_by_space(line);
        set<int> winning_numbers;   
        int card_score = 0;
        bool before_line = true;

        for(int i = 2; i < words.size(); i++){
            if(before_line){
                if(words[i] == "|"){
                    before_line = false;
                }else{
                    winning_numbers.emplace(stoi(words[i]));
                }
            }else{
                int check_number = stoi(words[i]);
                if(winning_numbers.count(check_number)){
                    card_score = card_score == 0 ? 1 : card_score * 2;
                }
            }
        }
        score += card_score;
    }
    cout << "Total Score: " << score;
}