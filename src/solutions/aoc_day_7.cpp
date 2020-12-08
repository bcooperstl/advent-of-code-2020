#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <regex>

#include "aoc_day_7.h"
#include "file_utils.h"

using namespace std;

BagContents::BagContents(int quantity, Bag * bag)
{
    m_quantity = quantity;
    m_bag = bag;
}

BagContents::~BagContents()
{
}

int BagContents::get_quantity()
{
    return m_quantity;
}

Bag * BagContents::get_bag()
{
    return m_bag;
}

Bag::Bag(string color)
{
    m_color = color;
    m_processed = false;
}

Bag::~Bag()
{
}

string Bag::get_color()
{
    return m_color;
}

bool Bag::get_processed()
{
    return m_processed;
}

vector<Bag *> Bag::get_contained_by()
{
    return m_contained_by;
}

vector<BagContents> Bag::get_contents()
{
    return m_contents;
}

void Bag::add_contained_by(Bag * bag)
{
    m_contained_by.push_back(bag);
}

void Bag::add_contents(int quantity, Bag * bag)
{
    m_contents.push_back(BagContents(quantity, bag));
}

void Bag::set_processed()
{
    m_processed = true;
}

regex AocDay7::no_other_bags_regex("(.*) bags contain no other bags\\.");
regex AocDay7::contains_bags_regex("(.*) bags contain (.*)");
regex AocDay7::contents_regex("(\\d+) ([a-z ]*) bags?[,\\.] ?");

AocDay7::AocDay7():AocDay(7)
{
}

AocDay7::~AocDay7()
{
    for (map<string, Bag *>::iterator bag_iter = m_lookup_map.begin(); bag_iter != m_lookup_map.end(); ++bag_iter)
    {
        delete bag_iter->second;
    }
    m_lookup_map.clear();
}

Bag * AocDay7::find_add_bag_in_map(string color)
{
    // Search for the color
    map<string, Bag *>::iterator lookup = m_lookup_map.find(color);
    if (lookup != m_lookup_map.end())
    {
        // if the color is found in the map, return the Bag
        return lookup->second;
    }
    // no color found - create a new bag, add it to the map, and return the new bag.
    Bag * bag = new Bag(color);
    cout << "Lookup Map: Bag " << bag->get_color() << " added without contents" << endl;
    m_lookup_map[color]=bag;
    return bag;
}

void AocDay7::process_line(string line)
{
#ifdef DEBUG_REGEX
    cout << "Processing line [" << line << "]" << endl;
#endif
    match_results<std::string::const_iterator> matches;
    
    // First search to see if this matches a line with no contents
    regex_search(line, matches, no_other_bags_regex);
    if (matches.size() > 0)
    {
#ifdef DEBUG_REGEX
        cout << "  No Other Bags regex matches. Color is " << matches[1] << endl;
#endif
        Bag * bag = find_add_bag_in_map(matches[1].str());
        cout << "No contents to add to " << matches[1].str() << " bag " << endl;
        // Nothing else to do here - no contents to add
        return;
    }
    else
    {
#ifdef DEBUG_REGEX
        cout << "  No Other Bags regex does not match" << endl;
#endif
    }
    
    regex_search(line, matches, contains_bags_regex);
    if (matches.size() > 0)
    {
#ifdef DEBUG_REGEX
        cout << "  Contains regex matches. Color is " << matches[1] << endl;
        cout << "  Remainder is [" << matches[2] << "]" << endl;
#endif
        Bag * bag = find_add_bag_in_map(matches[1].str());
        cout << "Adding contents to add to " << matches[1].str() << " bag " << endl;
        
        string contents = matches[2].str();
        int submatches[] = { 1, 2 };
        std::regex_token_iterator<std::string::iterator> end_iter;
        regex_token_iterator<std::string::iterator> contents_iter ( contents.begin(), contents.end(), contents_regex, submatches );
        while (contents_iter != end_iter)
        {
#ifdef DEBUG_REGEX
            cout << "    Quantity comes from " << *contents_iter << endl;
#endif
            int quantity = strtol((*contents_iter).str().c_str(), NULL, 10);
            ++contents_iter;
#ifdef DEBUG_REGEX
            cout << "    Color comes from " << *contents_iter << endl;
#endif
            string contents_color = (*contents_iter).str();
            ++contents_iter;
            Bag * contents_bag = find_add_bag_in_map(contents_color);
            cout << " Adding " << quantity << " " << contents_color << " bags to " << matches[1].str() << " bag." << endl;
            bag->add_contents(quantity, contents_bag);
            cout << " Adding " << matches[1].str() << " bag to be contained by " << contents_color << " bag." << endl;
            contents_bag->add_contained_by(bag);
        }
        return;
    }
    else
    {
#ifdef DEBUG_REGEX
        cout << "  Contains regex does not match, either. Time to fix your regexes" << endl;
#endif
    }
    return;
}

void AocDay7::process_input(string filename)
{
    FileUtils fileutils;
    vector<string> raw_lines;
    if (!fileutils.read_as_list_of_strings(filename, raw_lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return;
    }
    for (vector<string>::iterator iter = raw_lines.begin(); iter != raw_lines.end(); ++iter)
    {
        process_line(*iter);
    }
    return;
}

/*
Use the lookup table find the shiny gold Bag. From this, go through each member of the contained_by list. Mark them processed and add them to a list of container bags.  
Iterate through this container bags list, doing the same lookup and add to the end of list, but only if the looked up bag has not been processed.  
Repeat until every element in the list has been done.  
The length of this list is the result.
*/
string AocDay7::part1(string filename, vector<string> extra_args)
{
    process_input(filename);
    vector<Bag *> containing_bags;
    
    int counter = 0;
    // get the gold bag
    Bag * start = m_lookup_map["shiny gold"];
    
    // mark all of the gold back's contained_by bags as processed and add them to the containing_bags list
    vector<Bag *> contained_by = start->get_contained_by();
    cout << endl << endl << "Processing " << contained_by.size() << " bags from " << start->get_color() << " to get its containing bags" << endl;
    for (vector<Bag *>::iterator bag_iter = contained_by.begin(); bag_iter != contained_by.end(); ++bag_iter)
    {
        Bag * bag = *bag_iter;
        cout << " Marking " << bag->get_color() << " as processed; it is contained by " << start->get_color() << endl;
        bag->set_processed();
        containing_bags.push_back(bag);
    }
    
    // repeat the above process with every bag in the containing_bags list
    for (int cb_index=0; cb_index < containing_bags.size(); cb_index++)
    {
        Bag * cb = containing_bags[cb_index];
        cout << "Processing " << cb->get_color() << " to get its containing bags" << endl;
        contained_by = cb->get_contained_by();
        for (vector<Bag *>::iterator bag_iter = contained_by.begin(); bag_iter != contained_by.end(); ++bag_iter)
        {
            Bag * bag = *bag_iter;
            if (!bag->get_processed())
            {
                cout << " Marking " << bag->get_color() << " as processed; it is contained by " << cb->get_color() << endl;
                bag->set_processed();
                containing_bags.push_back(bag);
            }
            else
            {
                cout << " Skipping " << bag->get_color() << ". It is already processed" << endl;
            }
        }
    }
    
    ostringstream out;
    out << containing_bags.size();

    // Need to clean up my internal data before a next run
    for (map<string, Bag *>::iterator bag_iter = m_lookup_map.begin(); bag_iter != m_lookup_map.end(); ++bag_iter)
    {
        delete bag_iter->second;
    }
    m_lookup_map.clear();

    return out.str();
}

