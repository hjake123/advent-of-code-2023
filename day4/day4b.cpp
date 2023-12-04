#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>

#include "hyper.hpp"

using namespace std;

struct card{
    int count;
    vector<int> yield;
};

auto read_id(string id_word) -> int{
    return stoi(id_word.substr(0, id_word.size()-1));
}

auto read_cards(ifstream &infile, map<int, card> &cards){
    while (!infile.eof()){
        string line;
        getline(infile, line);
        vector<string> words = hyper::split_by_space(line);
        set<int> winning_numbers;
        bool before_line = true;
        int score = 0;

        for (int i = 2; i < words.size(); i++){
            if (before_line){
                if (words[i] == "|"){
                    before_line = false;
                }else{
                    winning_numbers.emplace(stoi(words[i]));
                }
            }else{
                int check_number = stoi(words[i]);
                if (winning_numbers.count(check_number)){
                    score++;
                }
            }
        }

        int id = read_id(words[1]);
        int reward = id + 1;
        card c;
        c.count = 1;
        for (int i = 0; i < score; i++){
            c.yield.push_back(reward++);
        }
        cards.emplace(id, c);
    }
}

int main(int argc, char **argv){
    auto infile = hyper::open(argc, argv, "input.txt");
    map<int, card> cards;
    read_cards(infile, cards);
    for(pair<int, card> p : cards){
        for(int i = 0; i < p.second.count; i++){
            for(int yielded_id : p.second.yield){
                cards[yielded_id].count += 1;
            }
        }
    }
    int total = 0;
    for(pair<int, card> p : cards){
        total += p.second.count;
    }
    cout << "Total ending cards: " << total;
}