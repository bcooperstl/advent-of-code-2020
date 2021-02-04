#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "aoc_day_22.h"
#include "file_utils.h"

#define DEBUG_DAY22

using namespace std;

Deck::Deck()
{
}

Deck::~Deck()
{
}

int Deck::pop_top_card()
{
    int top = m_cards.front();
    m_cards.pop_front();
    return top;
}

void Deck::push_card(int card)
{
    m_cards.push_back(card);
}

int Deck::get_num_cards()
{
    return m_cards.size();
}

int Deck::get_score()
{
    int score = 0;
    int position_multiplier = m_cards.size();
    for (deque<int>::iterator card = m_cards.begin(); card != m_cards.end(); ++card)
    {
        score += position_multiplier * (*card);
        position_multiplier--;
    }
    return score;
}

AocDay22::AocDay22():AocDay(22)
{
}

AocDay22::~AocDay22()
{
}


void AocDay22::parse_input(string filename, Deck & player1, Deck & player2)
{
    FileUtils fileutils;
    vector<string> lines;
    if (!fileutils.read_as_list_of_strings(filename, lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return;
    }
    
    vector<string>::iterator line_pos = lines.begin();
    ++line_pos; // skip the player 1 label
    // section 1 - fields
    while (*line_pos != "") // go until we have the blank line
    {
        string line = *line_pos;
        int value = strtol(line.c_str(), NULL, 10);
        player1.push_card(value);
        ++line_pos;
    }
    ++line_pos; // skip the blank line
    ++line_pos; // skip the player 2 label
    while (line_pos != lines.end()) // go until we have the blank line
    {
        string line = *line_pos;
        int value = strtol(line.c_str(), NULL, 10);
        player2.push_card(value);
        ++line_pos;
    }
    return;
}

/*
Planned approach

* While both players have cards in the deck
    * Pop the top card off of each player's deck
    * If player 1's card is larger:
        * Add player 1's card to the back of player 1's deck
        * Add player 2's card to the back of player 1's deck
    * else
        * Add player 2's card to the back of player 1's deck
        * Add player 1's card to the back of player 1's deck
*/

void AocDay22::play_game(Deck & player1, Deck & player2)
{
    int rounds = 0;
    while ((player1.get_num_cards() > 0) && (player2.get_num_cards() > 0))
    {
        rounds++;
        int card1 = player1.pop_top_card();
        int card2 = player2.pop_top_card();
        
        if (card1 > card2)
        {
#ifdef DEBUG_DAY22
            cout << "Round " << rounds << ": Player 1's " << card1 << " beats Player 2's " << card2 << endl;
#endif
            player1.push_card(card1);
            player1.push_card(card2);
        }
        else
        {
#ifdef DEBUG_DAY22
            cout << "Round " << rounds << ": Player 2's " << card1 << " beats Player 1's " << card2 << endl;
#endif
            player2.push_card(card2);
            player2.push_card(card1);
        }
    }
    cout << "The game is over after " << rounds << " rounds" << endl;
}

string AocDay22::part1(string filename, vector<string> extra_args)
{
    Deck player1, player2;
    parse_input(filename, player1, player2);
    play_game(player1, player2);
    
    int winning_score;
    if (player1.get_num_cards() > 0)
    {
        winning_score = player1.get_score();
        cout << "Player 1 wins with score=" << winning_score << endl;
    }
    else
    {
        winning_score = player2.get_score();
        cout << "Player 2 wins with score=" << winning_score << endl;
    }
    
    ostringstream out;
    out << winning_score;
    return out.str();
}

