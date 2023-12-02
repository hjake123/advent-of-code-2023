#include <iostream>
#include <fstream>

using namespace std;

int const MAX_RED = 12;
int const MAX_GREEN = 13;
int const MAX_BLUE = 14;

struct game_minima {
    int r = 0;
    int g = 0;
    int b = 0;
};

void adjust_round_minima(string const &round, game_minima &gm){
    string buffer;
    int count = 0;
    bool stage_read_count = true;
    bool last_turn = false;
    for(char c : round){
        if(last_turn){
            break;
        }
        if(c == ';'){
            last_turn = true;
        }
        if(stage_read_count){
            if(c == ' '){
                count = stoi(buffer);
                stage_read_count = false;
                buffer = "";
            }else{
                buffer.push_back(c);
            }
        }else{
            if(c == ',' || c == ';'){
                if(buffer == "red" && gm.r < count){
                    gm.r = count;
                }else if(buffer == "green" && gm.g < count){
                    gm.g = count;
                }else if(buffer == "blue" && gm.b < count){
                    gm.b = count;
                }
            }else if(c == ' '){
                stage_read_count = true;
                buffer = "";
            }else{
                buffer.push_back(c);
            }
        }
    }
    if(buffer == "red" && gm.r < count){
        gm.r = count;
    }else if(buffer == "green" && gm.g < count){
        gm.g = count;
    }else if(buffer == "blue" && gm.b < count){
        gm.b = count;
    }
}

auto compute_game_minima(string const &game) -> game_minima{
    game_minima gm;
    size_t next_round_index = game.find(':') + 2;
    while(next_round_index < game.size()){
        adjust_round_minima(game.substr(next_round_index), gm);
        size_t possible_semicolon_index = game.find(';', next_round_index);
        if(possible_semicolon_index == -1){
            break;
        }else{
            next_round_index = game.find(';', next_round_index) + 2;
        }
    }
    return gm;
}

auto power(game_minima const &gm) -> int{
    return gm.r * gm.g * gm.b;
}

int main(int argc, char **argv){
    string filename = argc == 1 ? "example.txt" : argv[1];
    ifstream input_file = ifstream(filename);
    if(!input_file.is_open()){
        cerr << "Couldn't find input file " << filename;
        exit(1);
    }
    int count = 0;
    while(!input_file.eof()){
        string line;
        getline(input_file, line);
        game_minima mins = compute_game_minima(line);
        int p = power(mins);
        clog << p << endl;
        count += p;
    }
    cout << "Sum: " << count;
}