#include <iostream>
#include <fstream>

using namespace std;
const string DIGITS = "0123456789";
const string NUMBER_NAMES[] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

int find_first_digit(const string &line){
    size_t digit_index = line.find_first_of(DIGITS);
    int digit;
    if(digit_index == -1){
        digit_index = line.size();
        digit = -1;
    }else{
        digit = line[digit_index] - (int) '0';
    }
    for(int i = 0; i < 10; i++){
        size_t found_index = line.find(NUMBER_NAMES[i]);
        if(found_index == -1)
            continue;
        if(found_index < digit_index){
            digit = i;
            digit_index = found_index;
        }
    }
    return digit;
}

int find_last_digit(const string &line){
    size_t digit_index = line.find_last_of(DIGITS);
    int digit;
    if(digit_index == -1){
        digit_index = -1;
        digit = -1;
    }else{
        digit = line[digit_index] - (int) '0';
    }
    for(int i = 0; i < 10; i++){
        size_t found_index = line.rfind(NUMBER_NAMES[i]);
        if(found_index == -1)
            continue;
        if(found_index > digit_index){
            digit = i;
            digit_index = found_index;
        }
    }
    return digit;
}

int main(int argc, char **argv){
    ifstream input_file = ifstream(argv[1]);
    int count = 0;
    if(!input_file.is_open()){
        cerr << "Couldn't find input file " << argv[1];
        return 1;
    }
    while(!input_file.eof()){
        string line;
        getline(input_file, line);
        int digit1, digit2;
        digit1 = find_first_digit(line);
        digit2 = find_last_digit(line);
        count += digit1*10 + digit2;
        cout << digit1 << " " << digit2 << endl;
    }
    cout << count;
}
