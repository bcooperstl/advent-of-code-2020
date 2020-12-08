#ifndef __AOC_DAY_7__
#define __AOC_DAY_7__

#include <regex>
#include <string>
#include <map>

#include "aoc_day.h"

using namespace std;

class Bag;

// Hoping in part 2 I need to do more to this; otherwise it could have just been a struct...or a pair object.
class BagContents
{
    private:
        Bag * m_bag;
        int m_quantity;
    public:
        BagContents(int quantity, Bag * bag);
        ~BagContents();
        int get_quantity();
        Bag * get_bag();
};

class Bag
{
private:
    string m_color;
    vector<Bag *> m_contained_by;
    vector<BagContents> m_contents;
    bool m_processed;
    int m_number_of_contained_bags;
public:
    Bag(string color);
    ~Bag();
    string get_color();
    bool get_processed();
    void set_number_of_contained_bags(int number);
    int get_number_of_contained_bags();
    vector<Bag *> get_contained_by();
    vector<BagContents> get_contents();
    void add_contained_by(Bag * bag);
    void add_contents(int quantity, Bag * bag);
    void set_processed();
};

class AocDay7 : public AocDay
{
    private:
        static regex no_other_bags_regex;
        static regex contains_bags_regex;
        static regex contents_regex;
        
        map<string, Bag *> m_lookup_map;
        void process_line(string line);
        void process_input(string filename);
        Bag * find_add_bag_in_map(string color);
    public:
        AocDay7();
        ~AocDay7();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};


#endif
