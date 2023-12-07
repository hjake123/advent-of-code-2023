#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "hyper.hpp"

using namespace std;
string const CARD_ORDER = "J23456789TQKA";

auto card_from_char(char c) -> int{
    return(CARD_ORDER.find(c));
}

class hand{
    // Hand types stored best to worst.
    enum hand_type{
        FIVE_OF_A_KIND,
        FOUR_OF_A_KIND,
        FULL_HOUSE,
        THREE_OF_A_KIND,
        TWO_PAIR,
        ONE_PAIR,
        HIGH_CARD
    };
    struct nmatch_result{
        bool match;
        int card;
        int jokers_used;
    };
protected:
    vector<int> sorted_cards;  
public:
    vector<int> cards;  
    hand_type type;
    int bid;
    hand(string const &line){
        vector<string> parts = hyper::split_by_space(line);
        for(char c : parts[0]){
            this->cards.push_back(card_from_char(c));
        }
        sorted_cards = cards;
        sort(sorted_cards.begin(), sorted_cards.end());
        this->bid = stoi(parts[1]);

        if(nmatch(5).match){
            this->type = FIVE_OF_A_KIND;
            return;
        }
        if(nmatch(4).match){
            this->type = FOUR_OF_A_KIND;
            return;
        }
        nmatch_result three_res = nmatch(3);
        if(three_res.match){
            if(nmatch(2, three_res.card, three_res.jokers_used).match){
                this->type = FULL_HOUSE;
            }else{
                this->type = THREE_OF_A_KIND;
            }
            return;
        }
        nmatch_result two_res = nmatch(2);
        if(two_res.match){
            if(nmatch(2, two_res.card, two_res.jokers_used).match){
                this->type = TWO_PAIR;
            }else{
                this->type = ONE_PAIR;
            }
            return;
        }
        this->type = HIGH_CARD;
    }
    /*
    Returns whether there are n matching cards.
    */
    nmatch_result nmatch(int const &n){
        return nmatch(n, -1, 0);
    }
    /*
    Returns whether there are n matching cards if you ignore cards of the excluded power.
    Now with joker management :`)
    */
    nmatch_result nmatch(int const &n, int const &exclude, int previous_jokers_used){
        int pow = -1;
        int run = 0;
        int jokers = 0;
        int best_run = 0;
        int best_run_pow = -1;
        for(int i = 0; i < sorted_cards.size(); i++){
            if(sorted_cards[i] == 0){ // Lookin' cool, joker!
                if(previous_jokers_used > 0){
                    previous_jokers_used--;
                }else{
                    jokers++;
                }
            }
            else if(sorted_cards[i] != pow){
                if(best_run < run){
                    best_run = run;
                    best_run_pow = pow;
                }
                pow = sorted_cards[i] == exclude ? -1 : sorted_cards[i];
                run = sorted_cards[i] == exclude ? 0 : 1;
            }else{
                run++;
            }
            if(run >= n){
                return nmatch_result{true, pow, 0};
            }
        }
        if(jokers + best_run >= n)
            return nmatch_result{true, best_run_pow, n - best_run};
        if(jokers + run >= n)
            return nmatch_result{true, pow, n - run};
        return nmatch_result{false, pow, 0};
    }
    bool operator<(hand const &other){
        if(other.type < this->type){
            return true;
        }
        if(other.type > this->type){
            return false;
        }
        for(int i = 0; i < cards.size(); i++){
            if(cards[i] < other.cards[i]){
                return true;
            }
            if(cards[i] > other.cards[i]){
                return false;
            }
        }
        throw logic_error("Can't compare exactly equal hands."); // If they are exactly equal, error out.
    }
};  

int main(int argc, char **argv){
    ifstream file = hyper::open(argc, argv, "example.txt");
    vector<hand> hands; // hands! of time will wring my neck.
    string line;
    while(!file.eof()){
        getline(file, line);
        hands.push_back(hand(line));
    }
    sort(hands.begin(), hands.end());
    int winnings = 0;
    for(int i = 0; i < hands.size(); i++){
        winnings += hands[i].bid * (i + 1);
    }
    cout << "Total winnings: " << winnings;
}