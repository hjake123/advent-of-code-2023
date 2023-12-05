#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include "hyper.hpp"

using namespace std;

class mapping_range{
private:
    long long displacement;
    long long range_start;
    long long range_end;
public:
    mapping_range(long long destination_head, long long source_head, long long range_size){
        range_start = source_head;
        range_end = source_head + range_size;
        displacement = destination_head - source_head;
    }
    auto is_in_range(long long n) -> long long{
        return n >= range_start && n < range_end;
    }
    auto lookup(long long n) -> long long {
        if(is_in_range(n))
            return n + displacement;
        return n;
    }
};  

class mapping{
private:
    vector<mapping_range> ranges;
public:
    void put(long long destination_head, long long source_head, long long range_size){
        mapping_range m(destination_head, source_head, range_size);
        put(m);
    }
    void put(mapping_range &r){
        ranges.push_back(r);
    }
    auto lookup(long long n) -> long long{
        for(mapping_range r : ranges){
            if(r.is_in_range(n)){
                return r.lookup(n);
            }
        }
        return n;
    }
};

void read_sequence(std::ifstream &file, std::string &line, std::vector<mapping> &map_sequence){
    vector<mapping_range> ranges;
    while (!file.eof()){
        getline(file, line);
        if (line.ends_with(':')){
            mapping m;
            for (mapping_range r : ranges){
                m.put(r);
            }
            map_sequence.push_back(m);
            ranges.clear();
        }else if (line.size() > 0){
            vector<long long> nums = hyper::parse_long_longs(line);
            mapping_range r(nums[0], nums[1], nums[2]);
            ranges.push_back(r);
        }
    }
    mapping m;
    for (mapping_range r : ranges){
        m.put(r);
    }
    map_sequence.push_back(m);
    ranges.clear();
}

int main(int argc, char **argv){
    ifstream file = hyper::open(argc, argv, "input.txt");
    string line;
    getline(file, line);
    vector<long long> seeds = hyper::parse_long_longs(line);
    
    // Consume the lines until the start of the first mapping.
    getline(file, line);
    getline(file, line);

    vector<mapping> map_sequence;
    read_sequence(file, line, map_sequence);    
    
    for(mapping m : map_sequence){
        for(int i = 0; i < seeds.size(); i++){
            seeds[i] = m.lookup(seeds[i]);
        }
    }

    long long min = LLONG_MAX;
    for(long long n : seeds){
        if(n < min)
            min = n;
    }
    cout << "Lowest location number: " << min;
}
