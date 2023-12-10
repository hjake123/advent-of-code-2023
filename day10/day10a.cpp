#include <iostream>
#include <fstream>
#include <vector>
#include <map>


#include "hyper.hpp"

using namespace std;

enum direction{
    NORTH, WEST, SOUTH, EAST
};

map<char, vector<direction>> pipe_to_dir
{
    {'|', vector<direction>{NORTH, SOUTH}},
    {'-', vector<direction>{WEST, EAST}},
    {'L', vector<direction>{NORTH, EAST}},
    {'J', vector<direction>{NORTH, WEST}},
    {'7', vector<direction>{WEST, SOUTH}},
    {'F', vector<direction>{SOUTH, EAST}},
    {'.', vector<direction>{}}
};

auto reverse(direction const &d) -> direction{
    switch(d){
        case NORTH:
            return SOUTH;
        case SOUTH:
            return NORTH;
        case WEST:
            return EAST;
        case EAST:
            return WEST;
    }
    cerr << "Invalid direction reversal!" << endl;
    return WEST;
}

void shift(int &x, int &y, direction const &d){
    switch(d){
        case NORTH:
            y -= 1;
            break;
        case WEST:
            x -= 1;
            break;
        case SOUTH:
            y += 1;
            break;
        case EAST:
            x += 1;
            break;
    }
} 

auto move_along_pipe(vector<vector<char>> const &grid, int &x, int &y, direction const &arrived_from) -> direction{
    for(direction to : pipe_to_dir[grid[y][x]]){
        if(to == arrived_from){
            continue;
        }
        shift(x, y, to);
        return reverse(to);
    }
    cerr << "invalid pipe movement!" << endl;
    return NORTH;
}

auto move_from_start(vector<vector<char>> const &grid, int &x, int &y) -> direction{
    for(int d = 0; d < 4; d++){
        int x1 = x;
        int y1 = y;
        shift(x1, y1, (direction) d);
        for(direction other : pipe_to_dir[grid[y1][x1]]){
            if(d == reverse(other)){
                shift(x, y, (direction) d);
                return other;
            }
        }
    }
    cerr << "invalid start placement!" << endl;
    return SOUTH;
}

int main(int argc, char **argv){
    ifstream file = hyper::open(argc, argv, "input.txt");

    vector<vector<char>> grid;
    string line;
    int x, y;
    while(getline(file, line)){
        grid.push_back(vector<char>());
        for(char c : line){
            grid.back().push_back(c);
            if(c == 'S'){
                x = grid.back().size() - 1;
                y = grid.size() - 1;
            }        
        }
    }
    //hyper::debug_print(grid);
    //cout << endl;
    direction from = move_from_start(grid, x, y);
    int steps = 1;
    while(grid[y][x] != 'S'){
        //cout << grid[y][x];
        from = move_along_pipe(grid, x, y, from);
        steps++;
    }
    //cout << endl;
    cout << "Half way should be " << steps/2;
}