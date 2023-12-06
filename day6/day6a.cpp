#include <iostream>
#include <fstream>
#include <vector>

#include "hyper.hpp"

using namespace std;

struct race{
    int time;
    int record;
};

void read_races(std::ifstream &file, std::vector<race> &races){
    string line;
    getline(file, line);
    for (int i : hyper::parse_ints(line)){
        race r;
        r.time = i;
        races.push_back(r);
    }
    getline(file, line);
    vector<int> dists = hyper::parse_ints(line);
    for (int i = 0; i < races.size(); i++){
        races[i].record = dists[i];
    }
}

// Does this race with this charge beat the record?
auto check(race r, int charge_time) -> bool{
    return charge_time * (r.time - charge_time) > r.record;
}

int main(int argc, char **argv){
    ifstream file = hyper::open(argc, argv, "example.txt");
    vector<race> races;
    read_races(file, races);
    // Against my better judgement, I will be baited into just brute forcing it. 
    // May part 2 have mercy on me...
    int score = 1;
    for(race r : races){
        int defeat_count = 0;
        for(int i = 1; i < r.time; i++){
            if(check(r, i)){
                defeat_count++;
            }
        }
        score *= defeat_count;
    }
    cout << "Score: " << score;
}   

