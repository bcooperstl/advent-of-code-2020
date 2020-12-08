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
    m_number_of_contained_bags = 0;
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

int Bag::get_number_of_contained_bags()
{
    return m_number_of_contained_bags;
}

void Bag::set_number_of_contained_bags(int number)
{
    m_number_of_contained_bags = number;
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
#ifdef DEBUG_DAY7_BAGS
    cout << "Lookup Map: Bag " << bag->get_color() << " added without contents" << endl;
#endif
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
#ifdef DEBUG_DAY7_BAGS
        cout << "No contents to add to " << matches[1].str() << " bag " << endl;
#endif
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
#ifdef DEBUG_DAY7_BAGS
        cout << "Adding contents to add to " << matches[1].str() << " bag " << endl;
#endif        
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
#ifdef DEBUG_DAY7_BAGS
            cout << " Adding " << quantity << " " << contents_color << " bags to " << matches[1].str() << " bag." << endl;
#endif
            bag->add_contents(quantity, contents_bag);
#ifdef DEBUG_DAY7_BAGS
            cout << " Adding " << matches[1].str() << " bag to be contained by " << contents_color << " bag." << endl;
#endif
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
#ifdef DEBUG_DAY7_BAGS
    cout << endl << endl << "Processing " << contained_by.size() << " bags from " << start->get_color() << " to get its containing bags" << endl;
#endif
    for (vector<Bag *>::iterator bag_iter = contained_by.begin(); bag_iter != contained_by.end(); ++bag_iter)
    {
        Bag * bag = *bag_iter;
#ifdef DEBUG_DAY7_BAGS
        cout << " Marking " << bag->get_color() << " as processed; it is contained by " << start->get_color() << endl;
#endif
        bag->set_processed();
        containing_bags.push_back(bag);
    }
    
    // repeat the above process with every bag in the containing_bags list
    for (int cb_index=0; cb_index < containing_bags.size(); cb_index++)
    {
        Bag * cb = containing_bags[cb_index];
#ifdef DEBUG_DAY7_BAGS
        cout << "Processing " << cb->get_color() << " to get its containing bags" << endl;
#endif
        contained_by = cb->get_contained_by();
        for (vector<Bag *>::iterator bag_iter = contained_by.begin(); bag_iter != contained_by.end(); ++bag_iter)
        {
            Bag * bag = *bag_iter;
            if (!bag->get_processed())
            {
#ifdef DEBUG_DAY7_BAGS
                cout << " Marking " << bag->get_color() << " as processed; it is contained by " << cb->get_color() << endl;
#endif
                bag->set_processed();
                containing_bags.push_back(bag);
            }
            else
            {
#ifdef DEBUG_DAY7_BAGS
                cout << " Skipping " << bag->get_color() << ". It is already processed" << endl;
#endif
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

/*
* Use the lookup table to find the shiny gold bag, and store this at the beginning of the to_process list.
* Loop over the to_process list, adding all of the Bags from that Bag's contents list to the to_process list. There will be duplicates, and that is ok. Eventually, the bags will have empty contents lists, so this will terminate.

* Loop over the to_process list in reverse. 
* If the Bag has already been processed, do nothing
* If not, calculate the number of contained bags by multiplying each quantity by the corresponding Bag's (number of contained bags + 1), and summing those results. The +1 is for the bag itself

* The shiny gold Bag's number of contained bags is the final result.
*/
string AocDay7::part2(string filename, vector<string> extra_args)
{
    process_input(filename);
    vector<Bag *> to_process;
    
    int counter = 0;
    // get the gold bag
    Bag * start = m_lookup_map["shiny gold"];
    
#ifdef DEBUG_DAY7_BAGS
    cout << endl << endl << "Adding " << start->get_color() << " to be processed as the starting bag" << endl;
#endif
    to_process.push_back(start);
    for (int i=0; i<to_process.size(); i++)
    {
        Bag * bag = to_process[i];
        vector<BagContents> contained_bags = bag->get_contents();
        //cout << "The " << bag->get_color() << " bag has " << contained_bags.size() << " contents" << endl;
        for (int j=0; j<contained_bags.size(); j++)
        {
#ifdef DEBUG_DAY7_BAGS
            cout << "Adding " << contained_bags[j].get_bag()->get_color() << " to be processed from " << bag->get_color() << endl;
#endif
            to_process.push_back(contained_bags[j].get_bag());
        }
    }
    
#ifdef DEBUG_DAY7_BAGS
    cout << "Processing in reverse" << endl;
#endif
    for (int i=to_process.size() - 1; i >= 0; i--)
    {
        Bag * bag = to_process[i];
        // skip if already processed
        if (bag->get_processed())
        {
#ifdef DEBUG_DAY7_BAGS
            cout << "Bag " << bag->get_color() << " is already processed" << endl;
#endif
            continue;
        }
        vector<BagContents> contents = bag->get_contents();
        int sum = 0;
        for (int j=0; j<contents.size(); j++)
        {
            Bag * contained_bag = contents[j].get_bag();
            if (!contained_bag->get_processed())
            {
                cout << "****** BAG " << contained_bag->get_color() << " HAS NOT BEEN PROCESSED. CHECK YOUR LOGIC ******" << endl;
            }
            sum += (contents[j].get_quantity() * (contained_bag->get_number_of_contained_bags() + 1));
        }
#ifdef DEBUG_DAY7_BAGS
        cout << "Setting bag " << bag->get_color() << " to have " << sum << " contained bags" << endl;
#endif
        bag->set_number_of_contained_bags(sum);
        bag->set_processed();
    }
        
    ostringstream out;
    out << start->get_number_of_contained_bags();

    // Need to clean up my internal data before a next run
    for (map<string, Bag *>::iterator bag_iter = m_lookup_map.begin(); bag_iter != m_lookup_map.end(); ++bag_iter)
    {
        delete bag_iter->second;
    }
    m_lookup_map.clear();

    return out.str();
}
