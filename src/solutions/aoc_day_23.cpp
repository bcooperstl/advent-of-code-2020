#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <map>

#include "aoc_day_23.h"
#include "file_utils.h"

//#define DEBUG_DAY23

using namespace std;

Game::Game()
{
    for (int i=0; i<NUM_CUPS; i++)
    {
        m_cups[i] = new Cup();
        m_cups[i]->next = NULL;
        m_cups[i]->prev = NULL;
        if (i == 0)
        {
            m_cups[i]->label=NUM_CUPS;
        }
        else
        {
            m_cups[i]->label=i;
        }
        m_cups[i]->picked_up=false;
    }
    m_current = 0;
}

Game::~Game()
{
    for (int i=0; i<NUM_CUPS; i++)
    {
        delete m_cups[i];
    }
}

void Game::init(string input)
{
    m_current = m_cups[(input[0]-'0')%NUM_CUPS];
#ifdef DEBUG_DAY23
    cout << "Initializing game. First current cup is " << m_current->label << endl;
#endif
    for (int i=0; i<NUM_CUPS; i++)
    {
        int value = input[i]-'0';
        int next_value = input[(i+1)%NUM_CUPS]-'0';
        int prev_value = input[(i+NUM_CUPS-1)%NUM_CUPS]-'0';
#ifdef DEBUG_DAY23
        cout << "Setting " << prev_value << " --> " << value << " --> " << next_value << endl;
#endif
        m_cups[value%NUM_CUPS]->next=m_cups[next_value%NUM_CUPS];
        m_cups[value%NUM_CUPS]->prev=m_cups[prev_value%NUM_CUPS];
    }
}

void Game::display()
{
    Cup * c = m_current;
    cout << "(" << c->label << ") ";
    c = c->next;
    while (c != m_current)
    {
        cout << c->label << " ";
        c = c->next;
    }
    cout << endl;
}

void Game::play_round()
{
#ifdef DEBUG_DAY23
    cout << "Playing a round starting with current cup " << m_current->label << endl;
#endif
    
    Cup * picked_up[3];
    picked_up[0]=m_current->next;
    picked_up[1]=picked_up[0]->next;
    picked_up[2]=picked_up[1]->next;
    
    m_current->next = picked_up[2]->next;
    picked_up[2]->next->prev = m_current;
    
#ifdef DEBUG_DAY23
    cout << " Picked up " << picked_up[0]->label << "," << picked_up[1]->label << "," << picked_up[2]->label << endl;
#endif
    
    Cup * dest = m_cups[(m_current->label+NUM_CUPS-1)%NUM_CUPS];
    while (dest == picked_up[0] || dest == picked_up[1] || dest == picked_up[2])
    {
        dest = m_cups[(dest->label+NUM_CUPS-1)%NUM_CUPS];
    }
#ifdef DEBUG_DAY23
    cout << " Destination is after " << dest->label << endl;
#endif
    picked_up[2]->next=dest->next;
    dest->next->prev=picked_up[2];
    dest->next=picked_up[0];
    picked_up[0]->prev=dest;
    m_current = m_current->next;
#ifdef DEBUG_DAY23
    cout << " next current cup is " << m_current->label << endl;
#endif
}

void Game::play_rounds(int num_rounds)
{
    for (int i=0; i<num_rounds; i++)
    {
        play_round();
        display();
    }
}

string Game::get_part1_answer()
{
    char ret[9];
    ret[8]='\0';
    Cup * current = m_cups[1]->next;
    for (int i=0; i<8; i++)
    {
        ret[i]='0'+(current->label);
        current = current->next;
    }
    return string(ret);
}

BigGame::BigGame()
{
    m_cups = new BigGameCup[BIG_NUM_CUPS];
    m_current = NULL;
    for (int i=0; i<BIG_NUM_CUPS; i++)
    {
        m_cups[i].next = NULL;
        if (i == 0)
        {
            m_cups[i].label=BIG_NUM_CUPS;
        }
        else
        {
            m_cups[i].label=i;
        }
    }
}

BigGame::~BigGame()
{
    delete [] m_cups;
}

void BigGame::display()
{
    BigGameCup * c = m_current;
    cout << "(" << c->label << ") " << endl;
    c = c->next;
    while (c != m_current)
    {
        cout << c->label << " " << endl;
        c = c->next;
    }
    cout << endl;
}

void BigGame::init(string input)
{
    m_current = &m_cups[(input[0]-'0')%NUM_CUPS];
#ifdef DEBUG_DAY23
    cout << "Initializing game. First current cup is " << m_current->label << endl;
#endif
    for (int i=0; i<NUM_CUPS-1; i++) // the first 8 places
    {
        int value = input[i]-'0';
        int next_value = input[(i+1)%NUM_CUPS]-'0';
#ifdef DEBUG_DAY23
        cout << "Setting " << value << " --> " << next_value << endl;
#endif
        m_cups[value].next=&m_cups[next_value];
    }
    
    // 9th place - goes from the 9th input to 10
    int value = input[8]-'0';
    m_cups[value].next=&m_cups[10];
#ifdef DEBUG_DAY23
        cout << "Setting " << value << " --> " << 10 << endl;
#endif
    
    // places 10 through BIG_NUM_CUPS-1 - goes to the next one
    for (int i=10; i<BIG_NUM_CUPS-1; i++)
    {
        m_cups[i].next=&m_cups[i+1];
#ifdef DEBUG_DAY23
        cout << "Setting " << i << " --> " << i+1 << endl;
#endif
    }
    
    // BIG_NUM_CUPS-1 goes to BIG_NUM_CUPS, which is at 0
    m_cups[BIG_NUM_CUPS-1].next=&m_cups[0];
#ifdef DEBUG_DAY23
        cout << "Setting " << BIG_NUM_CUPS-1 << " --> " << BIG_NUM_CUPS << endl;
#endif
    // BIG_NUM_CUPS - goes back to the first cup
    m_cups[0].next=m_current;
#ifdef DEBUG_DAY23
        cout << "Setting " << BIG_NUM_CUPS << " --> " << m_current->label << endl;
#endif
    //display();
}    

void BigGame::play_rounds(int num_rounds)
{
    for (int i=0; i<num_rounds; i++)
    {
        if (i%100000 == 0)
        {
            cout << "Round " << i << endl;
        }
    #ifdef DEBUG_DAY23
        cout << "Playing a round starting with current cup " << m_current->label << endl;
    #endif
        
        BigGameCup * picked_up[3];
        picked_up[0]=m_current->next;
        picked_up[1]=picked_up[0]->next;
        picked_up[2]=picked_up[1]->next;
        
        m_current->next = picked_up[2]->next;
        
    #ifdef DEBUG_DAY23
        cout << " Picked up " << picked_up[0]->label << "," << picked_up[1]->label << "," << picked_up[2]->label << endl;
    #endif
        
        // Might have to speed this up - not sure how long the mods will take
        int dest_option;
        BigGameCup * dest = NULL;
        dest_option = m_current->label;
        for (int j=0; j<4; j++)
        {
            dest_option--;
            if (dest_option < 0)
            {
                dest_option+=BIG_NUM_CUPS;
            }
            dest = &m_cups[dest_option];
            if (dest != picked_up[0] && dest != picked_up[1] && dest != picked_up[2])
            {
                break;
            }
        }
        
    #ifdef DEBUG_DAY23
        cout << " Destination is after " << dest->label << endl;
    #endif
        picked_up[2]->next=dest->next;
        dest->next=picked_up[0];
        m_current = m_current->next;
    #ifdef DEBUG_DAY23
        cout << " next current cup is " << m_current->label << endl;
    #endif
    }
}

long BigGame::get_part2_answer()
{
    BigGameCup * first = m_cups[1].next;
    BigGameCup * second = first->next;
    
    return ((long)first->label) * ((long)second->label);
}

AocDay23::AocDay23():AocDay(23)
{
}

AocDay23::~AocDay23()
{
}


string AocDay23::get_input(string filename)
{
    FileUtils fileutils;
    vector<string> lines;
    if (!fileutils.read_as_list_of_strings(filename, lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return "";
    }
    
    return lines[0];
}

string AocDay23::part1(string filename, vector<string> extra_args)
{
    if (extra_args.size() != 1)
    {
        cerr << "Day 23 Part 1 required 1 extra argument for the number of rounds)" << endl;
        return "";
    }
    
    Game game;
    string input = get_input(filename);
    
    int num_rounds = strtol(extra_args[0].c_str(), NULL, 10);
    
    game.init(input);
    game.display();
    game.play_rounds(num_rounds);
    
    ostringstream out;
    out << game.get_part1_answer();
    return out.str();
}

string AocDay23::part2(string filename, vector<string> extra_args)
{
    if (extra_args.size() != 1)
    {
        cerr << "Day 23 Part 2 required 1 extra argument for the number of rounds)" << endl;
        return "";
    }
    
    BigGame game;
    string input = get_input(filename);
    
    int num_rounds = strtol(extra_args[0].c_str(), NULL, 10);
    
    game.init(input);
    //game.display();
    game.play_rounds(num_rounds);
    
    ostringstream out;
    out << game.get_part2_answer();
    return out.str();
}
