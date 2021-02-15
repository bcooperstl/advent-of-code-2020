#ifndef __AOC_DAY_23__
#define __AOC_DAY_23__

#include <deque>
#include <string>

#include "aoc_day.h"

using namespace std;

#define NUM_CUPS 9

struct Cup
{
    Cup * next;
    Cup * prev;
    int label;
    bool picked_up;
};

class Game
{
private:
    Cup * m_cups[NUM_CUPS];
    Cup * m_current;
public:    
    Game();
    ~Game();
    void init(string input);
    void play_round();
    void play_rounds(int num_rounds);
    void display();
    string get_part1_answer();
};

class AocDay23 : public AocDay
{
    private:
        string get_input(string filename);
    public:
        AocDay23();
        ~AocDay23();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};


#endif
