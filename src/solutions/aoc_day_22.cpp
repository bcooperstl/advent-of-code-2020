#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <map>

#include "aoc_day_22.h"
#include "file_utils.h"

//#define DEBUG_DAY22

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

// Key for a deck is (number of cards*100)+(top card value)
int Deck::get_deck_key()
{
    if (m_cards.size() > 0)
    {
        return ((m_cards.size() * 100) + m_cards.front());
    }
    return 0;
}

deque<int> Deck::get_current_cards()
{
    return m_cards;
}

bool Deck::matches(deque<int> target)
{
    if (m_cards.size() != target.size())
    {
        return false;
    }
    for (int i=0; i<m_cards.size(); i++)
    {
        if (m_cards[i] != target[i])
        {
            return false;
        }
    }
    return true;
}

Deck Deck::get_recursive_deck(int num_cards)
{
    Deck recursive;
    for (int i=0; i<num_cards; i++)
    {
        recursive.push_card(m_cards[i]);
    }
    return recursive;
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

int AocDay22::get_game_key(Deck & player1, Deck & player2)
{
    return ((player1.get_deck_key() * 10000) + player2.get_deck_key());
}

// returns which player is the winner of the game
int AocDay22::play_recursive_game(int depth, Deck & player1, Deck & player2)
{
    map<int, vector<pair<deque<int>, deque<int>>>> card_history_map;
    int rounds = 0;
    while ((player1.get_num_cards() > 0) && (player2.get_num_cards() > 0))
    {
        rounds++;
        cout << "Playing at depth " << depth << " round " << rounds << endl;
        // before drawing, make sure state hasn't already been seen.
        int game_key = get_game_key(player1, player2);
        map<int, vector<pair<deque<int>, deque<int>>>>::iterator map_pos = card_history_map.find(game_key);
#ifdef DEBUG_DAY22
        cout << " Checking history for key " << game_key << endl;
#endif
        
        if (map_pos != card_history_map.end())
        {
#ifdef DEBUG_DAY22
            cout << "  History for this key exists" << endl;
#endif
            vector<pair<deque<int>, deque<int>>> histories = map_pos->second;
            for (int hist_pos=0; hist_pos<histories.size(); hist_pos++)
            {
#ifdef DEBUG_DAY22
                cout << "  Checking history " << hist_pos+1 << endl;
#endif
                if (player1.matches(histories[hist_pos].first) && player2.matches(histories[hist_pos].second))
                {
                    cout << "   Match found...Returning a player 1 victory" << endl;
                    return 1;
                }
            }
#ifdef DEBUG_DAY22
            cout << "  No matching history found" << endl;
#endif
        }
        else
        {
#ifdef DEBUG_DAY22
            cout << " No history for this key exists. Adding to lookup map" << endl;
#endif
            card_history_map[game_key]=vector<pair<deque<int>, deque<int>>>();
        }
        
        card_history_map[game_key].push_back(pair<deque<int>, deque<int>>(player1.get_current_cards(), player2.get_current_cards()));
        
        int card1 = player1.pop_top_card();
        int card2 = player2.pop_top_card();
        int size1 = player1.get_num_cards();
        int size2 = player2.get_num_cards();
        
#ifdef DEBUG_DAY22
        cout << "Player 1 drew " << card1 << " and has " << size1 << " cards remaining. Player 2 drew " << card2 << " and has " << size2 << " cards remaining" << endl;
#endif
        
        if ((size1 >= card1) && (size2 >= card2))
        {
#ifdef DEBUG_DAY22
            cout << " Depth " << depth << " starting recursive combat" << endl;
#endif
            Deck recursive1 = player1.get_recursive_deck(card1);
            Deck recursive2 = player2.get_recursive_deck(card2);
            int winner = play_recursive_game(depth+1, recursive1, recursive2);
#ifdef DEBUG_DAY22
            cout << " Depth " << depth << " got " << winner << " as the winner of recursive combat" << endl;
#endif
            if (winner == 1)
            {
                player1.push_card(card1);
                player1.push_card(card2);
            }
            else            
            {
                player2.push_card(card2);
                player2.push_card(card1);
            }
        }
        else
        {
#ifdef DEBUG_DAY22
            cout << " Non-recursive combat" << endl;
#endif
            if (card1 > card2)
            {
#ifdef DEBUG_DAY22
                cout << "  Player 1's " << card1 << " beats Player 2's " << card2 << endl;
#endif
                player1.push_card(card1);
                player1.push_card(card2);
            }
            else
            {
#ifdef DEBUG_DAY22
                cout << "  Player 2's " << card1 << " beats Player 1's " << card2 << endl;
#endif
                player2.push_card(card2);
                player2.push_card(card1);
            }
        }
    }
    int winner = 0;
    if (player1.get_num_cards() == 0)
    {
        winner = 2;
        cout << " Player 2 wins because player 1 is out of cards " << endl;
    }
    else if (player2.get_num_cards() == 0)
    {
        winner = 1;
        cout << " Player 1 wins because player 2 is out of cards " << endl;
    }
    else
    {
        cout << "***LOOK INTO ERROR - neither player out of cards***" << endl;
    }
    return winner;
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

string AocDay22::part2(string filename, vector<string> extra_args)
{
    Deck player1, player2;
    parse_input(filename, player1, player2);
    int winner = play_recursive_game(1, player1, player2);
    
    int winning_score;
    if (winner == 1)
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

