#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <map>
#include <set>

#include "aoc_day_21.h"
#include "file_utils.h"

#define DEBUG_DAY21

using namespace std;

Allergen::Allergen(string name)
{
    m_name = name;
    m_done = false;
    m_matched_ingredient = NULL;
    m_possible_ingredients.clear();
}

Allergen::~Allergen()
{
}

string Allergen::get_name()
{
    return m_name;
}

bool Allergen::is_done()
{
    return m_done;
}

Ingredient * Allergen::get_matched_ingredient()
{
    return m_matched_ingredient;
}

vector<Ingredient *> Allergen::get_possible_ingredients()
{
    return m_possible_ingredients;
}

Ingredient::Ingredient(string name)
{
    m_name = name;
    m_done = false;
    m_matched_allergen = NULL;
    m_possible_allergens.clear();
}

Ingredient::~Ingredient()
{
}

string Ingredient::get_name()
{
    return m_name;
}

bool Ingredient::is_done()
{
    return m_done;
}

Allergen * Ingredient::get_matched_allergen()
{
    return m_matched_allergen;
}

vector<Allergen *> Ingredient::get_possible_allergens()
{
    return m_possible_allergens;
}

Food::Food()
{
    m_allergens.clear();
    m_ingredients.clear();
}

Food::~Food()
{
}

void Food::add_allergen(Allergen * allergen)
{
    m_allergens.push_back(allergen);
}

vector<Allergen *> Food::get_all_allergens()
{
    return m_allergens;
}

vector<Allergen *> Food::get_not_done_allergens()
{
    vector<Allergen *> ret;
    for (int i=0; i<m_allergens.size(); i++)
    {
        if (!m_allergens[i]->is_done())
        {
            ret.push_back(m_allergens[i]);
        }
    }
    return ret;
}

bool Food::has_allergen(Allergen * allergen)
{
    for (int i=0; i<m_allergens.size(); i++)
    {
        if (m_allergens[i] == allergen)
        {
            return true;
        }
    }
    return false;
}

void Food::add_ingredient(Ingredient * ingredient)
{
    m_ingredients.push_back(ingredient);
}

vector<Ingredient *> Food::get_all_ingredients()
{
    return m_ingredients;
}

vector<Ingredient *> Food::get_not_done_ingredients()
{
    vector<Ingredient *> ret;
    for (int i=0; i<m_ingredients.size(); i++)
    {
        if (!m_ingredients[i]->is_done())
        {
            ret.push_back(m_ingredients[i]);
        }
    }
    return ret;
}

bool Food::has_ingredient(Ingredient * ingredient)
{
    for (int i=0; i<m_ingredients.size(); i++)
    {
        if (m_ingredients[i] == ingredient)
        {
            return true;
        }
    }
    return false;
}

AocDay21::AocDay21():AocDay(21)
{
}

AocDay21::~AocDay21()
{
}

void AocDay21::cleanup(vector<Food *> & foods, vector<Allergen *> & allergens, vector<Ingredient *> & ingredients)
{
    for (int i=0; i<foods.size(); i++)
    {
        delete foods[i];
    }
    foods.clear();

    for (int i=0; i<allergens.size(); i++)
    {
        delete allergens[i];
    }
    allergens.clear();

    for (int i=0; i<ingredients.size(); i++)
    {
        delete ingredients[i];
    }
    ingredients.clear();
}    

/* Planned approach:
For each line that is parsed, I will look until I get the string `(contains`.  
Items before that string are ingredients.  
Items after that string are allergens, but the final character (either a `,` or `)`) needs to be stripped off.

Sample line:
mxmxvkd kfcds sqjhc nhms (contains dairy, fish)

*/

void AocDay21::parse_input(string filename, vector<Food *> & foods, vector<Allergen *> & allergens, vector<Ingredient *> & ingredients)
{
    foods.clear();
    allergens.clear();
    ingredients.clear();
    
    FileUtils fileutils;
    vector<vector<string>> split_lines;
    
    map<string, Allergen *> allergen_lookup;
    map<string, Ingredient *> ingredient_lookup;
    
    if (!fileutils.read_as_list_of_split_strings(filename, split_lines, ' ', '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return;
    }
    
    // Line format is 
    for (int line=0; line<split_lines.size(); line++)
    {
#ifdef DEBUG_DAY21
        cout << "Parsing line " << line << endl;
#endif
        bool in_allergens = false;
        vector<string> parts = split_lines[line];
        Food * food = new Food();
        
        for (int i=0; i<parts.size(); i++)
        {
            if (parts[i] == "(contains")
            {
                in_allergens = true;
#ifdef DEBUG_DAY21
                cout << " Switching to Allergens" << endl;
#endif
            }
            else if (!in_allergens)
            {
                map<string, Ingredient *>::iterator lookup_result = ingredient_lookup.find(parts[i]);
                Ingredient * ingredient = NULL;
                if (lookup_result == ingredient_lookup.end())
                {
#ifdef DEBUG_DAY21
                    cout << " Adding Ingredient " << parts[i] << " as a new Ingredient" << endl;
#endif
                    // Not in map; need to create it
                    ingredient = new Ingredient(parts[i]);
                    ingredient_lookup[parts[i]] = ingredient;
                    ingredients.push_back(ingredient);
                }
                else
                {
                    ingredient = lookup_result->second;
#ifdef DEBUG_DAY21
                    cout << " Using existing Ingredient " << ingredient->get_name() << endl;
#endif
                }
                food->add_ingredient(ingredient);
            }
            else
            {
                string name = parts[i].substr(0, parts[i].length()-1);
                map<string, Allergen *>::iterator lookup_result = allergen_lookup.find(name);
                Allergen * allergen = NULL;
                if (lookup_result == allergen_lookup.end())
                {
#ifdef DEBUG_DAY21
                    cout << " Adding Allergen " << name << " as a new Allergen" << endl;
#endif
                    // Not in map; need to create it
                    allergen = new Allergen(name);
                    allergen_lookup[name] = allergen;
                    allergens.push_back(allergen);
                }
                else
                {
                    allergen = lookup_result->second;
#ifdef DEBUG_DAY21
                    cout << " Using existing Allergen " << allergen->get_name() << endl;
#endif
                }
                food->add_allergen(allergen);
            }
        }
        foods.push_back(food);
    }
    return;
}


string AocDay21::part1(string filename, vector<string> extra_args)
{
    vector<Food *> foods;
    vector<Allergen *> allergens;
    vector<Ingredient *> ingredients;
    
    parse_input(filename, foods, allergens, ingredients);
    
    cleanup(foods, allergens, ingredients);
    
    ostringstream out;
    out << "";
    return out.str();
}

