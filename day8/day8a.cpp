#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include "hyper.hpp"

using namespace std;

struct node{
public:
    string name, left_name, right_name;
    node *left, *right;
};

auto read_graph(std::ifstream &file) -> node*{
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
    for (pair p : node_build_map){
        p.second->left = node_build_map[p.second->left_name];
        p.second->right = node_build_map[p.second->right_name];
    }
    return node_build_map["AAA"];
}

int main(int argc, char **argv){
    ifstream file = hyper::open(argc, argv, "input.txt");
    string instructions;
    getline(file, instructions);

    node *start = read_graph(file);
    node *current = start;
    int steps = 0;
    while(true){
        for(char c : instructions){
            if(current->name == "ZZZ"){
                cout << "Took " << steps << " steps to leave";
                return 0;
            }
            steps++;
            current = c == 'L' ? current->left : current->right;
        }
    }
}