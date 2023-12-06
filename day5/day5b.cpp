#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include "hyper.hpp"

using namespace std;

class range{
public:
    long long start;
    long long end;
    bool shifted_this_round; // Please turn this on and off at the right times!
    range(long long head, long long range_size){
        start = head;
        end = head + range_size;
        shifted_this_round = false;
    }
    auto is_in_range(long long n) -> long long{
        return n >= start && n < end;
    }
};

class mapping_range : public range{
private:
    long long displacement;
public:
    mapping_range(long long destination_head, long long source_head, long long range_size) : range(source_head, range_size){
        displacement = destination_head - source_head;
    }
    auto shift(range const &r) -> range{
        range s(r.start + this->displacement, r.end - r.start);
        s.shifted_this_round = true;
        return s;
    }
    /*
    Divide the given range into multiple modified ranges based on parts which are greater, less, or within this range.
    */
    auto split_and_move(range const &subject) -> vector<range>{
        vector<range> ret;
        if(subject.start >= this->end || subject.end <= this->start){
            ret.push_back(subject);
            return ret;
        }
        if(this->start <= subject.start && subject.end <= this->end){
            ret.push_back(this->shift(subject));
            return ret;
        }
        if(this->start <= subject.start && this->end <= subject.end){
            ret.push_back(this->shift(range(subject.start, this->end - subject.start)));
            ret.push_back(range(this->end, subject.end - this->end));
            return ret;
        }
        if(subject.start <= this->start && subject.end <= this->end){
            ret.push_back(range(subject.start, this->start - subject.start));
            ret.push_back(this->shift(range(this->start, subject.end - this->start)));
            return ret;
        }
        if(subject.start <= this->start && this->end <= subject.end){
            ret.push_back(this->shift((range) *this));
            ret.push_back(range(subject.start, this->start - subject.start));
            ret.push_back(range(this->end, subject.end - this->end));
            return ret;
        }
        throw logic_error("Range splitting missed case");
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
    void process(vector<range> &input){
        for(mapping_range mr : ranges){
            vector<range> processed;
            while(input.size() > 0){
                range r = input.back();
                input.pop_back();
                if(r.shifted_this_round){
                    processed.push_back(r);
                    continue;
                }
                vector<range> p = mr.split_and_move(r);
                for(range ra : p){
                    processed.push_back(ra);
                }
            }
            for(range ran : processed){
                input.push_back(ran);
            }
        }
        for(int i = 0; i < input.size(); i++){
            input[i].shifted_this_round = false;
        }
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

auto make_seed_ranges(vector<long long> seeds) -> vector<range>{
    vector<range> ranges;
    for(int i = 0; i < seeds.size(); i += 2){
        range r(seeds[i], seeds[i + 1]);
        ranges.push_back(r);
    }
    return ranges;
}

int main(int argc, char **argv){
    ifstream file = hyper::open(argc, argv, "example.txt");
    string line;
    getline(file, line);
    vector<range> ranges = make_seed_ranges(hyper::parse_long_longs(line));
    
    getline(file, line);
    getline(file, line);

    vector<mapping> map_sequence;
    read_sequence(file, line, map_sequence);    

    for(mapping m : map_sequence){
        m.process(ranges);
    }

    long long min = LLONG_MAX;
    for(range r : ranges){
        if(min > r.start){
            min = r.start;
        }
    }
    cout << "Lowest number location: " << min;
}
