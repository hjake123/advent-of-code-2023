#include <iostream>
#include <fstream>
#include <vector>

#include "hyper.hpp"

using namespace std;

struct race{
    long long time;
    long long record;
};

void read_race(std::ifstream &file, race &race){
    string line;
    getline(file, line);
    string buffer;
    for(char c : line){
        if(isdigit(c)){
            buffer.push_back(c);
        }
    }
    race.time = stoll(buffer);
    buffer = "";
    getline(file, line);
    for(char c : line){
        if(isdigit(c)){
            buffer.push_back(c);
        }
    }
    race.record = stoll(buffer);
}

// Does this race with this charge beat the record?
auto check(race r, long long charge_time) -> bool{
    return charge_time * (r.time - charge_time) > r.record;
}

int main(int argc, char **argv){
    ifstream file = hyper::open(argc, argv, "input.txt");
    race r;
    read_race(file, r);
    long long score = 0;
    // I mean if it works it works right?
    for(long long ll = 1; ll < r.time; ll++){
        if(check(r, ll)){
            score++;
        }
    }
    cout << "Score: " << score;
}   

