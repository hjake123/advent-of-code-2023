#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <numeric>


#include "hyper.hpp"

using namespace std;

struct node{
public:
    string name, left_name, right_name;
    node *left, *right;
};

auto read_graph(std::ifstream &file, vector<node*> &starts) -> int{
    string line;
    map<string, node*> node_build_map;
    getline(file, line);
    while (getline(file, line)){
        vector<string> tokens = hyper::split_by_space(line);
        node *n = new node();
        n->name = tokens[0];
        n->left_name = tokens[2].substr(1, 3);
        n->right_name = tokens[3].substr(0, 3);
        node_build_map.emplace(n->name, n);
    }
    int endcount;
    for (pair p : node_build_map){
        p.second->left = node_build_map[p.second->left_name];
        p.second->right = node_build_map[p.second->right_name];
        if(p.second->name.back() == 'A'){
            starts.push_back(p.second);
        }
        if(p.second->name.back() == 'Z'){
            endcount++;
        }
    }

    return endcount;
}

int main(int argc, char **argv){
    ifstream file = hyper::open(argc, argv, "b_example.txt");
    string instructions;
    getline(file, instructions);

    vector<node*> positions;
    int end_count = read_graph(file, positions);
    long long steps = 0;
    long long estimated_steps = 0;
    int ends_reached = 0;
    while(true){
        for(char c : instructions){
            for(int i = 0; i < positions.size(); i++){
                if(positions[i]->name.back() == 'Z'){
                    if(estimated_steps == 0)
                        estimated_steps = steps;
                    else
                        estimated_steps = lcm(estimated_steps, steps);
                    ends_reached++;
                    if(ends_reached == end_count){
                        cout << "Estimating " << estimated_steps << " steps";
                        return 0;
                    }
                }
                positions[i] = c == 'L' ? positions[i]->left : positions[i]->right;
            }
            steps++;
        }
    }
}