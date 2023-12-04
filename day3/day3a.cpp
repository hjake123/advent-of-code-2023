#include <iostream>
#include <fstream>
#include <vector>

#include "hyper.hpp"

using namespace std;

void read_schematic(ifstream &infile,vector<vector<char>> &schematic){
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

void mark_hitmap(vector<vector<char>> &schematic, vector<vector<bool>> &hitmap){
    for(int y = 0; y < schematic.size(); y++){
        vector<bool> hitline;
        for(int x = 0; x < schematic[y].size(); x++){
            hitline.push_back(false);
        }
        hitmap.push_back(hitline);
    }
    for(int y = 0; y < schematic.size(); y++){
        for(int x = 0; x < schematic[y].size(); x++){
            char glyph = schematic[y][x];
            if(glyph != '.' && !isdigit(glyph)){
                // There is a symbol here; overwrite the 3x3 area to true.
                for(int ix = x - 1; ix <= x + 1; ix++){
                    for(int iy = y - 1; iy <= y + 1; iy++){
                        hitmap[iy][ix] = true;
                    }
                }
            }
        }
    }
}

auto find_part_numbers(vector<vector<char>> &schematic, vector<vector<bool>> &hitmap) -> vector<int>{
    vector<int> part_numbers;
    string buffer;
    bool in_hit = false;
    for(int y = 0; y < schematic.size(); y++){
        for(int x = 0; x < schematic[y].size(); x++){
            char glyph = schematic[y][x];
            if(isdigit(glyph)){
                in_hit = hitmap[y][x] || in_hit;
                buffer.push_back(glyph);
            }else if(in_hit){
                in_hit = false;
                part_numbers.push_back(stoi(buffer));
                buffer = "";
            }else{
                buffer = "";
            }
        }
    }
    return part_numbers;
}

int main(int argc, char **argv){
    auto infile = hyper::open(argc, argv, "input.txt");

    // First, read the schematic into memory for easier manipulation.
    vector<vector<char>> schematic;
    read_schematic(infile, schematic);
    //debug_print(schematic);

    // Mark up a grid of spaces adjacent to symbols.
    vector<vector<bool>> hitmap;
    mark_hitmap(schematic, hitmap);
    //debug_print(hitmap);

    cout << hyper::sum(find_part_numbers(schematic, hitmap));

}