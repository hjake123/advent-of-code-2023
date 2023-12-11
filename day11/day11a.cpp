#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include "hyper.hpp"

using namespace std;

void expand(vector<string> &grid){
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
    // Now expand the rows.
    int rows_added = 0;
    for(int row : y_to_expand){
        string line = "";
        for(char c : grid[row]){
            line.push_back('.');
        }
        grid.insert(grid.begin() + row + rows_added, line);
        rows_added++;
    }
    // Now expand the columns.
    int cols_added = 0;
    for(int col : x_to_expand){
        for(int y = 0; y < grid.size(); y++){
            grid[y].insert(grid[y].begin() + col + cols_added, '.');
        }
        cols_added++;
    }
}

auto find_galactic_positions(vector<string> const &grid) -> vector<pair<int, int>>{
    vector<pair<int, int>> pos_list;
    for(int y = 0; y < grid.size(); y++){
        for(int x = 0; x < grid[y].size(); x++){
            if(grid[y][x] == '#'){
                pos_list.push_back(pair(x, y));
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
    expand(grid);
    int cum_distance = 0;
    const auto positions = find_galactic_positions(grid);
    for(size_t i = 0; i < positions.size(); i++){
        for(size_t j = i + 1; j < positions.size(); j++){
            auto const &pos1 = positions[i];
            auto const &pos2 = positions[j];
            int d = abs(pos2.second - pos1.second) + abs(pos2.first - pos1.first);
            cum_distance += d;
            // clog << i << ", " << j <<  ": " << d << endl;
        }
    }
    cout << cum_distance << endl;
}