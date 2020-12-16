#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iterator>

#include "aoc_day_11.h"
#include "file_utils.h"
#include "screen.h"

using namespace std;

#define FLOOR '.'
#define EMPTY_SEAT 'L'
#define OCCUPIED_SEAT '#'


AocDay11::AocDay11():AocDay(11)
{
}

AocDay11::~AocDay11()
{
}

vector<string> AocDay11::read_input(string filename)
{
    FileUtils fileutils;
    vector<string> raw_lines;
    if (!fileutils.read_as_list_of_strings(filename, raw_lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return raw_lines;
    }
    return raw_lines;
}

/*
* Read in the input file into a vector of strings using my existing function.,
* Create an empty Screen object, and store this in *prior*
* Create a Screen object from the input, and store this in *current*
* While *current* is not equal to *prior*
    * Copy *current* to *prior*
    * Loop over every column and row in *current* with *x* and *y*
        * If `prior[x][y]` is an empty seat (`L`)
            * If there are no adjacent occupied seats
                * Set current[x][y] to be occupied (`#`)
        * If `prior[x][y]` is an occupied seat (`#`)
            * If there are four or more adjacent occupied seats
                * Set current[x][y] to be empty (`L`)
* Count the number of occupied seats in the steady-state map and return that value.
*/

string AocDay11::part1(string filename, vector<string> extra_args)
{
    vector<string> lines = read_input(filename);
    Screen prior(FLOOR);
    Screen current(FLOOR);
    
    current.load(lines);
    
    int round=0;
    
    while (!current.equals(prior))
    {
        cout << "Iteration " << round++ << ":" << endl;
        current.display();
        cout << endl;
        prior = current;
        for (int y=current.get_min_y(); y<=current.get_max_y(); y++)
        {
            for (int x=current.get_min_x(); x<=current.get_max_x(); x++)
            {
                char ch = prior.get(x,y);
                if (ch == EMPTY_SEAT)
                {
                    if (prior.num_matching_neighbors(x,y,OCCUPIED_SEAT) == 0)
                    {
                        current.set(x,y,OCCUPIED_SEAT);
                    }
                }
                else if (ch == OCCUPIED_SEAT)
                {
                    if (prior.num_matching_neighbors(x,y,OCCUPIED_SEAT) >= 4)
                    {
                        current.set(x,y,EMPTY_SEAT);
                    }
                }
            }
        }
    }
    
    cout << "Final steady state:" << endl;
    current.display();
    
    ostringstream out;
    out << current.num_matching(OCCUPIED_SEAT);
    return out.str();
}

char AocDay11::find_next_seat(Screen & grid, int start_x, int start_y, int delta_x, int delta_y)
{
    //cout << start_x << " " << start_y << " " << delta_x << " " << delta_y << endl;
    int x = start_x+delta_x;
    int y = start_y+delta_y;
    while (x >= grid.get_min_x() && x <= grid.get_max_x() && y >= grid.get_min_y() && y <= grid.get_max_y())
    {
        //cout << x << " " << y << endl;
        char ch = grid.get(x,y);
        if (ch != FLOOR)
        {
            return ch;
        }
        x += delta_x;
        y += delta_y;
    }
    // got to the wall. Can just return the floor here
    return FLOOR;
}
    

int AocDay11::count_line_of_sight_occupied_seats(Screen & grid, int pos_x, int pos_y)
{
    int count = 0;
    // left
    if (find_next_seat(grid, pos_x, pos_y, -1, 0) == OCCUPIED_SEAT)
    {
        count++;
    }
    // right
    if (find_next_seat(grid, pos_x, pos_y, 1, 0) == OCCUPIED_SEAT)
    {
        count++;
    }
    // up
    if (find_next_seat(grid, pos_x, pos_y, 0, -1) == OCCUPIED_SEAT)
    {
        count++;
    }
    // down
    if (find_next_seat(grid, pos_x, pos_y, 0, 1) == OCCUPIED_SEAT)
    {
        count++;
    }
    // up-left
    if (find_next_seat(grid, pos_x, pos_y, -1, -1) == OCCUPIED_SEAT)
    {
        count++;
    }
    // up-right
    if (find_next_seat(grid, pos_x, pos_y, 1, -1) == OCCUPIED_SEAT)
    {
        count++;
    }
    // down-left
    if (find_next_seat(grid, pos_x, pos_y, -1, 1) == OCCUPIED_SEAT)
    {
        count++;
    }
    // down-right
    if (find_next_seat(grid, pos_x, pos_y, 1, 1) == OCCUPIED_SEAT)
    {
        count++;
    }
    return count;
}

/*
* Read in the input file into a vector of strings using my existing function.,
* Create an empty Screen object, and store this in *prior*
* Create a Screen object from the input, and store this in *current*
* While *current* is not equal to *prior*
    * Copy *current* to *prior*
    * Loop over every column and row in *current* with *x* and *y*
        * If `prior[x][y]` is an empty seat (`L`)
            * If there are no line-of-sight occupied seats
                * Set current[x][y] to be occupied (`#`)
        * If `prior[x][y]` is an occupied seat (`#`)
            * If there are five or more line-of-site occupied seats
                * Set current[x][y] to be empty (`L`)
* Count the number of occupied seats in the steady-state map and return that value.
*/
string AocDay11::part2(string filename, vector<string> extra_args)
{
    vector<string> lines = read_input(filename);
    Screen prior(FLOOR);
    Screen current(FLOOR);
    
    current.load(lines);
    
    int round=0;
    
    while (!current.equals(prior))
    {
        cout << "Iteration " << round++ << ":" << endl;
        current.display();
        cout << endl;
        prior = current;
        for (int y=current.get_min_y(); y<=current.get_max_y(); y++)
        {
            for (int x=current.get_min_x(); x<=current.get_max_x(); x++)
            {
                char ch = prior.get(x,y);
                if (ch == EMPTY_SEAT)
                {
                    if (count_line_of_sight_occupied_seats(prior,x,y) == 0)
                    {
                        current.set(x,y,OCCUPIED_SEAT);
                    }
                }
                else if (ch == OCCUPIED_SEAT)
                {
                    if (count_line_of_sight_occupied_seats(prior,x,y) >= 5)
                    {
                        current.set(x,y,EMPTY_SEAT);
                    }
                }
            }
        }
    }
    
    cout << "Final steady state:" << endl;
    current.display();
    
    ostringstream out;
    out << current.num_matching(OCCUPIED_SEAT);
    return out.str();
}
