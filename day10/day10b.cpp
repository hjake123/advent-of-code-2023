#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <queue>

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

void flood_fill(vector<vector<char>> &loop_mark){
    int x = 0;
    int y = 0;

    queue<pair<int, int>> todo;
    todo.push(pair(x, y));
    loop_mark[y][x] = 'O';
    while(todo.size() > 0){
        pair<int, int> pos = todo.front();
        todo.pop();

        for(int dir = NORTH; dir < 4; dir++){
            int off_x = pos.first;
            int off_y = pos.second;
            shift(off_x, off_y, (direction) dir);
            if(off_x >= 0 && off_x < loop_mark.back().size() 
            && off_y >= 0 && off_y < loop_mark.size()
            && loop_mark[off_y][off_x] == '.'){
                loop_mark[off_y][off_x] = 'O';
                todo.push(pair(off_x, off_y));
            }
        }
    }
}

int main(int argc, char **argv){
    ifstream file = hyper::open(argc, argv, "example4.txt");
    
    /*
    
    */
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

    /*
    Tracks whether each cell is the loop, and will be floodfilled later.
    '.' means 'not in the loop'
    '#' means 'the loop'
    'O' means 'out of the loop'
    */
    vector<vector<char>> loop_mark;
    for(int i = 0; i < grid.size() + 2; i++){
        loop_mark.push_back(vector<char>());
        for(int j = 0; j < grid.back().size() + 2; j++){
            loop_mark.back().push_back('.');
        }
    }
    hyper::debug_print(loop_mark);
    cout << endl;

    // Trace the loop and mark the various tables.
    loop_mark[y+1][x+1] = '#';
    direction from = move_from_start(grid, x, y);
    while(grid[y][x] != 'S'){
        loop_mark[y+1][x+1] = '#';
        from = move_along_pipe(grid, x, y, from);
    }
    hyper::debug_print(loop_mark);
    cout << endl;

    // Perform the super flood fill.
    flood_fill(loop_mark);
    hyper::debug_print(loop_mark);
    cout << endl;

    int unfilled = 0;
    for(vector<char> row: loop_mark){
        for(char c : row){
            if(c == '.'){
                unfilled++;
            }
        }
    }
    cout << unfilled << endl;
}