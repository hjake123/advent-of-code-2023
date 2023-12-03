#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>

#include "hyper.hpp"
#include "day3a.h"

using namespace std;

void read_schematic(ifstream &infile, vector<vector<char>> &schematic){
    while (!infile.eof())
    {
        string line;
        getline(infile, line);
        vector<char> linechars;
        for (char c : line)
        {
            linechars.push_back(c);
        }
        schematic.push_back(linechars);
    }
}

void mark_gear_grid(vector<vector<char>> &schematic, vector<vector<vector<int>>> &gear_grid){
    for(int y = 0; y < schematic.size(); y++){
        vector<vector<int>> gline;
        for(int x = 0; x < schematic[y].size(); x++){
            vector<int> v;
            gline.push_back(v);
        }
        gear_grid.push_back(gline);
    }
    int current_id = 1;
    for(int y = 0; y < schematic.size(); y++){
        for(int x = 0; x < schematic[y].size(); x++){
            char glyph = schematic[y][x];
            if(glyph == '*'){
                // There is a gear here; overwrite the 3x3 area to its id.
                for(int ix = x - 1; ix <= x + 1; ix++){
                    for(int iy = y - 1; iy <= y + 1; iy++){
                        gear_grid[iy][ix].push_back(current_id);
                    }
                }
                current_id++;
            }
        }
    }
}

auto find_gear_ratio_sum(vector<vector<char>> &schematic, vector<vector<vector<int>>> &gear_grid) -> int{
    map<int, vector<int>> gear_number_map;
    string buffer;
    set<int> in_gears;

    // Collect the numbers associated to each gear number in the grid into a map
    for(int y = 0; y < schematic.size(); y++){
        for(int x = 0; x < schematic[y].size(); x++){
            char glyph = schematic[y][x];
            if(isdigit(glyph)){
                for(int gear_id : gear_grid[y][x]){
                    in_gears.emplace(gear_id);
                }
                buffer.push_back(glyph);
            }else if(in_gears.size() > 0){
                for(int gear_id : in_gears){
                    if(gear_number_map.count(gear_id)){
                        gear_number_map[gear_id].push_back(stoi(buffer));
                    }else{
                        vector<int> v;
                        v.push_back(stoi(buffer));
                        gear_number_map.emplace(gear_id, v);
                    }
                }
                in_gears.clear();
                buffer = "";
            }else{
                buffer = "";
            }
        }
    }

    // For each entry in the map, if there are two entries, multiply them and add them to the running count.
    int count = 0;
    for(pair p : gear_number_map){
        if(p.second.size() == 2){
            count += p.second[0] * p.second[1];
        }
    }
    return count;
}

int main(int argc, char **argv){
    auto infile = hyper_open(argc, argv, "example.txt");

    // First, read the schematic into memory for easier manipulation.
    vector<vector<char>> schematic;
    read_schematic(infile, schematic);

    // Mark up a grid of gear ids to assosiate numbers to.
    vector<vector<vector<int>>> gear_grid;
    mark_gear_grid(schematic, gear_grid);

    cout << "Sum of gear ratios: " << find_gear_ratio_sum(schematic, gear_grid);

}