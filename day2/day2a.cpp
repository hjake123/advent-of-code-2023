#include <iostream>
#include <fstream>

using namespace std;

int const MAX_RED = 12;
int const MAX_GREEN = 13;
int const MAX_BLUE = 14;

auto validate_round(string const &round) -> bool{
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
                if(buffer == "red" && (count > MAX_RED) 
                || buffer == "green" && (count > MAX_GREEN)
                || buffer == "blue" && (count > MAX_BLUE)){
                    clog << "IMPOSSIBLE " << buffer << " with " << count << endl;
                    return false;
                }
            }else if(c == ' '){
                stage_read_count = true;
                buffer = "";
            }else{
                buffer.push_back(c);
            }
        }
    }
    if(buffer == "red" && (count > MAX_RED) 
    || buffer == "green" && (count > MAX_GREEN)
    || buffer == "blue" && (count > MAX_BLUE)){
        clog << "IMPOSSIBLE " << buffer << " with " << count << endl;
        return false;
    }
    return true;
}

auto validate_game(string const &game) -> bool{
    size_t next_round_index = game.find(':') + 2;
    while(next_round_index < game.size()){
        if(!validate_round(game.substr(next_round_index))){
            return false;
        }
        size_t possible_semicolon_index = game.find(';', next_round_index);
        if(possible_semicolon_index == -1){
            break;
        }else{
            next_round_index = game.find(';', next_round_index) + 2;
        }
    }
    return true;
}

auto read_game_id(string const &game) -> int{
    size_t i = 5;
    string buffer;
    while(isdigit(game[i])){
        buffer.push_back(game[i++]);
    }
    return stoi(buffer);
}

int main(int argc, char **argv){
    string filename = argc == 1 ? "hyper_example.txt" : argv[1];
    ifstream input_file = ifstream(filename);
    if(!input_file.is_open()){
        cerr << "Couldn't find input file " << filename;
        return 1;
    }
    int count = 0;
    while(!input_file.eof()){
        string line;
        getline(input_file, line);
        if(validate_game(line)){
            int id = read_game_id(line);
            clog << id << " possible!" << endl;
            count += id;
        }
    }
    cout << "Sum: " << count;
}