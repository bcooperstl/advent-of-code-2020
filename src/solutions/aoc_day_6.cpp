#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iterator>
#include <cstdint>
#include <bitset>

#include "aoc_day_6.h"
#include "file_utils.h"

using namespace std;

#define ANY_YESES_INIT 0
#define ALL_YESES_INIT 0xffffffff
#define PERSON_YESES_INIT 0
#define YES 0x01

Person::Person(string response)
{
    m_response = response;
    m_yeses = PERSON_YESES_INIT;
    
    for (int i=0; i<response.length(); i++)
    {
        // a will correspond to the least significant bit. the left shift is 0
        // z will correspond to the most significant bit. the left shift is 25
        m_yeses = m_yeses | (YES << (response[i]-'a'));
    }
#ifdef DEBUG_BITOPS
cout << "Person with responses of " << response << " results in " << std::bitset<32>(m_yeses) << endl;
#endif
}

Person::~Person()
{
}

string Person::get_response()
{
    return m_response;
}

uint32_t Person::get_yeses()
{
    return m_yeses;
}

Group::Group()
{
    m_group_any_yeses = ANY_YESES_INIT;
    m_group_all_yeses = ALL_YESES_INIT;
}

Group::~Group()
{
    for (int i=0; i<m_persons.size(); i++)
    {
        delete m_persons[i];
    }
    m_persons.clear();
}

void Group::add_person(Person * person)
{
#ifdef DEBUG_BITOPS
    cout << "Pre: " << std::bitset<32>(m_group_any_yeses) << "  ";
    cout << "Adding person with " << std::bitset<32>(person->get_yeses()) << "  ";
    cout << "Pre: " << std::bitset<32>(m_group_all_yeses) << "  ";
    cout << "Adding person with " << std::bitset<32>(person->get_yeses()) << "  ";
#endif

    m_persons.push_back(person);
    m_group_any_yeses = m_group_any_yeses | person->get_yeses();
    m_group_all_yeses = m_group_all_yeses & person->get_yeses();


#ifdef DEBUG_BITOPS
    cout << "Post: " << std::bitset<32>(m_group_any_yeses) << endl;
    cout << "Post: " << std::bitset<32>(m_group_all_yeses) << endl;
#endif
}

uint32_t Group::get_group_any_yeses()
{
    return m_group_any_yeses;
}

uint32_t Group::get_group_all_yeses()
{
    return m_group_all_yeses;
}

int Group::get_any_yes_count()
{
   int count = 0;
   for (int i=0; i<26; i++)
   {
       if (m_group_any_yeses & (YES << i))
       {
           count++;
       }
   }
#ifdef DEBUG_BITOPS
   cout << "Group with: " << std::bitset<32>(m_group_any_yeses) << " has count " << count << endl;
#endif
return count;
}

int Group::get_all_yes_count()
{
   int count = 0;
   for (int i=0; i<26; i++)
   {
       if (m_group_all_yeses & (YES << i))
       {
           count++;
       }
   }
#ifdef DEBUG_BITOPS
   cout << "Group with: " << std::bitset<32>(m_group_all_yeses) << " has count " << count << endl;
#endif
return count;
}


AocDay6::AocDay6():AocDay(6)
{
}

AocDay6::~AocDay6()
{
}

/*
* Create a new Group
* Loop over the lines of input
    * If a blank line is detected
        * Append the current Group object to the list of groups
        * Create a new Group object for the next entry
    * Else (not a blank line)
        * Create a new Person from the line
        * Add that person to the current Group
* Append the last Group object to the list.
*/
vector<Group *> AocDay6::parse_input(string filename)
{
    FileUtils fileutils;
    vector<string> raw_lines;
    vector<Group *> groups;
    if (!fileutils.read_as_list_of_strings(filename, raw_lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return groups;
    }
    
    Group * current = new Group();
    for (vector<string>::iterator iter = raw_lines.begin(); iter != raw_lines.end(); ++iter)
    {
        string line = *iter;
        if (line.length() == 0)
        {
            groups.push_back(current);
            current = new Group();
        }
        else
        {
            Person * person = new Person(line);
            current->add_person(person);
        }
    }
    
    groups.push_back(current);
    
    return groups;
}

string AocDay6::part1(string filename, vector<string> extra_args)
{
    vector<Group * > groups = parse_input(filename);
    int total_yeses = 0;
    
    for (vector<Group *>::iterator group_iter = groups.begin(); group_iter != groups.end(); ++group_iter)
    {
        Group * group = *group_iter;
        total_yeses += group->get_any_yes_count();
        // Done with the group. Delete it here so I don't write another clean-up loop
        delete group;
    }
    ostringstream out;
    out << total_yeses;
    return out.str();
}

string AocDay6::part2(string filename, vector<string> extra_args)
{
    vector<Group * > groups = parse_input(filename);
    int total_yeses = 0;
    
    for (vector<Group *>::iterator group_iter = groups.begin(); group_iter != groups.end(); ++group_iter)
    {
        Group * group = *group_iter;
        total_yeses += group->get_all_yes_count();
        // Done with the group. Delete it here so I don't write another clean-up loop
        delete group;
    }
    ostringstream out;
    out << total_yeses;
    return out.str();
}
