#include <iostream>
#include <fstream>
#include <vector>

#include "hyper.hpp"

using namespace std;

int main(int argc, char **argv){
    ifstream file = hyper::open(argc, argv, "input.txt");
}