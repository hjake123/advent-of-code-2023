#include <iostream>
#include <fstream>
#include <vector>
#include <list>


#include "hyper.hpp"

using namespace std;

void read_one_grid(ifstream &file, vector<string> &row_grid, vector<string> &col_grid){
    string line;
    while(getline(file, line)){
        if(line == ""){
            break;
        }
        row_grid.push_back(line);
    }
    for(int i = 0; i < row_grid.front().size(); i++){
        col_grid.push_back("");
    }
    for(int i = 0; i < row_grid.front().size(); i++){
        for(string row : row_grid){
            col_grid[i].push_back(row[i]);
        }
    }
}

auto find_reflection(vector<string> const &grid) -> size_t{
    list<string> past_lines;
    for(size_t i = 0; i < grid.size(); i++){
        string line = grid[i];
        size_t j = i;
        bool reflected = past_lines.size() > 0;
        for(string past : past_lines){
            if(j >= grid.size()){
                break;
            }
            if(grid[j] != past){
                reflected = false;
                break;
            }
            j++;
        }
        if(reflected){
            return i;
        }
        past_lines.push_front(line);
    }
    return -1;
}   

int main(int argc, char **argv){
    ifstream file = hyper::open(argc, argv, "input.txt");
    int col_sum = 0;
    int row_sum = 0;
    while(!file.eof()){
        vector<string> row_grid, col_grid;
        read_one_grid(file, row_grid, col_grid);
        size_t reflection = find_reflection(col_grid);
        if(reflection == -1){
            reflection = find_reflection(row_grid);
            row_sum += reflection;
        }else{
            col_sum += reflection;
        }
        if(reflection == -1){
            cerr << "No reflection." << endl;
            return 0;
        }
    }
    cout << "Summary: " << 100 * row_sum + col_sum << endl;
}