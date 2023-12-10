#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <queue>

#include "hyper.hpp"

using namespace std;

enum direction{
    NORTH, WEST, SOUTH, EAST
};

map<char, set<direction>> pipe_to_dir
{
    {'|', set<direction>{NORTH, SOUTH}},
    {'-', set<direction>{WEST, EAST}},
    {'L', set<direction>{NORTH, EAST}},
    {'J', set<direction>{NORTH, WEST}},
    {'7', set<direction>{WEST, SOUTH}},
    {'F', set<direction>{SOUTH, EAST}},
    {'.', set<direction>{}}
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

// Call with position of center for the region.
auto move_along_pipe(vector<vector<char>> &grid, int &x, int &y, direction const &arrived_from) -> direction{
    for(direction to : pipe_to_dir[grid[y][x]]){
        if(to == arrived_from){
            continue;
        }
        grid[y][x] = 'X';
        shift(x, y, to);
        grid[y][x] = 'X';
        shift(x, y, to);
        grid[y][x] = 'X';
        shift(x, y, to);

        return reverse(to);
    }
    cerr << "invalid pipe movement!" << endl;
    return NORTH;
}

auto move_from_start(vector<vector<char>> &grid, int &x, int &y) -> direction{
    for(int d = 0; d < 4; d++){
        int x1 = x;
        int y1 = y;
        shift(x1, y1, (direction) d);
        shift(x1, y1, (direction) d);
        shift(x1, y1, (direction) d);
        if(y1 < 0 || y1 >= grid.size() || x1 < 0 || x1 >= grid[y1].size()){
            continue;
        }
        for(direction other : pipe_to_dir[grid[y1][x1]]){
            if(d == reverse(other)){
                shift(x, y, (direction) d);
                grid[y][x] = 'X';
                shift(x, y, (direction) d);
                grid[y][x] = 'X';
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
            && loop_mark[off_y][off_x] != 'X'
            && loop_mark[off_y][off_x] != 'O'){
                loop_mark[off_y][off_x] = 'O';
                todo.push(pair(off_x, off_y));
            }
        }
    }
}

int main(int argc, char **argv){
    ifstream file = hyper::open(argc, argv, "example4.txt");
    
    /*
    Tracks whether each cell is the loop, and will be floodfilled later.
    '.' means 'inside the loop' (overwritten by floodfill)
    '#' means 'piece of pipe' (overwritten by floodfill)
    'X' means 'part of the loop' (blocks floodfill)
    'O' means 'out of the loop' (applied in floodfill)
    Anything else is the character from the original input file.

    Each 3x3 region is a segment of pipe from one tile in the original, so count in a special way at the end.
    All pipes will have at least # in the middle of their region.
    */
    vector<vector<char>> grid;
    string line;
    int start_x, start_y;
    while(getline(file, line)){
        grid.push_back(vector<char>());
        grid.push_back(vector<char>());
        grid.push_back(vector<char>());
        for(char c : line){
            if(c == 'S'){
                start_y = grid.size() - 2;
                start_x = grid[grid.size()-2].size() + 1;
            }
            set<direction> dirs = pipe_to_dir[c];

            // Fill top row.
            grid[grid.size()-3].push_back('.');
            dirs.count(NORTH) > 0 ? grid[grid.size()-3].push_back('#') : grid[grid.size()-3].push_back('.');
            grid[grid.size()-3].push_back('.');

            // Fill middle row.
            dirs.count(WEST) > 0 ? grid[grid.size()-2].push_back('#') : grid[grid.size()-2].push_back('.');
            grid[grid.size()-2].push_back(c);
            dirs.count(EAST) > 0 ? grid[grid.size()-2].push_back('#') : grid[grid.size()-2].push_back('.');

            // Fill bottom row.
            grid.back().push_back('.');
            dirs.count(SOUTH) > 0 ? grid.back().push_back('#') : grid.back().push_back('.');
            grid.back().push_back('.');
        }
    }

    // Fix start.
    for(int d = 0; d < 4; d++){
        int x1 = start_x;
        int y1 = start_y;
        shift(x1, y1, (direction) d);
        int x2 = x1;
        int y2 = y1;
        shift(x2, y2, (direction) d);
        if(y2 >= 0 && y2 < grid.size() && x2 >= 0 && x2 < grid[y2].size() && grid[y2][x2] == '#'){
            grid[y1][x1] = '#';
        }
    }
    hyper::debug_print(grid);
    cout << endl;

    // Trace over the loop.
    direction from = move_from_start(grid, start_x, start_y);
    while(grid[start_y][start_x] != 'S'){
        from = move_along_pipe(grid, start_x, start_y, from);
    }
    grid[start_y][start_x] = 'X';

    // Perform the super flood fill.
    flood_fill(grid);

    int unfilled = 0;
    for(int y = 1; y < grid.size(); y += 3){
        for(int x = 1; x < grid.back().size(); x += 3){
            if(grid[y][x] != 'O' && grid[y][x] != 'X'){
                grid[y][x] = 'I';
                unfilled++;
            }
        }
    }
    hyper::debug_print(grid);
    cout << endl;
    cout << unfilled << endl;
}