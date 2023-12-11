#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include "hyper.hpp"

using namespace std;

auto get_expansions(vector<string> &grid) -> pair<vector<int>, vector<int>>{
    // First get the rows.
    vector<int> x_to_expand, y_to_expand;
    for(int y = 0; y < grid.size(); y++){
        bool expand_this_row = true;
        for(int x = 0; x < grid[y].size(); x++){
            expand_this_row = expand_this_row && grid[y][x] == '.';
        }
        if(expand_this_row)
            y_to_expand.push_back(y);
    }
    // Now get the columns.
    for(int x = 0; x < grid.front().size(); x++){
        bool expand_this_col = true;
        for(int y = 0; y < grid.size(); y++){
            expand_this_col = expand_this_col && grid[y][x] == '.';
        }
        if(expand_this_col)
            x_to_expand.push_back(x);
    }
    return pair(x_to_expand, y_to_expand);
}

auto find_galactic_positions(vector<string> const &grid, pair<vector<int>, vector<int>> const &expansions) -> vector<pair<long long, long long>>{
    vector<pair<long long, long long>> pos_list;
    const int EXPANSION = 999999; // Should be amount to expand by - 1.
    for(int y = 0; y < grid.size(); y++){
        for(int x = 0; x < grid[y].size(); x++){
            if(grid[y][x] == '#'){
                long long x_offset = 0;
                for(int col : expansions.first){
                    if(col > x){
                        break;
                    }
                    x_offset += EXPANSION;
                }
                long long y_offset = 0;
                for(int row : expansions.second){
                    if(row > y){
                        break;
                    }
                    y_offset += EXPANSION;
                }
                
                pos_list.push_back(pair(x + x_offset, y + y_offset));
            }
        }
    }
    return pos_list;
}

int main(int argc, char **argv){
    ifstream file = hyper::open(argc, argv, "input.txt");
    vector<string> grid;
    string line;
    while(getline(file, line)){
        grid.push_back(line);
    }
    const auto positions = find_galactic_positions(grid, get_expansions(grid));

    long long cum_distance = 0;
    for(size_t i = 0; i < positions.size(); i++){
        for(size_t j = i + 1; j < positions.size(); j++){
            auto const &pos1 = positions[i];
            auto const &pos2 = positions[j];
            long long d = abs(pos2.second - pos1.second) + abs(pos2.first - pos1.first);
            cum_distance += d;
        }
    }
    cout << cum_distance << endl;
}