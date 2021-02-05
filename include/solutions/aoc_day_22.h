#ifndef __AOC_DAY_22__
#define __AOC_DAY_22__

#include <deque>
#include <string>

#include "aoc_day.h"

using namespace std;

class Deck 
{
    private:
        deque<int> m_cards;
    public:
        Deck();
        ~Deck();
        int pop_top_card();
        void push_card(int card);
        int get_num_cards();
        int get_score();
        int get_deck_key();
        deque<int> get_current_cards();
        Deck get_recursive_deck(int num_cards);
        bool matches(deque<int> target);
};

class AocDay22 : public AocDay
{
    private:
        void parse_input(string filename, Deck & player1, Deck & player2);
        void play_game(Deck & player1, Deck & player2);
        int get_game_key(Deck & player1, Deck & player2);
        int play_recursive_game(int depth, Deck & player1, Deck & player2);
    public:
        AocDay22();
        ~AocDay22();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};


#endif
