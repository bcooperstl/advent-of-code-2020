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

void Allergen::set_done(bool done)
{
    m_done = done;
}

bool Allergen::is_done()
{
    return m_done;
}

void Allergen::set_matched_ingredient(Ingredient * matched)
{
    m_matched_ingredient = matched;
}

Ingredient * Allergen::get_matched_ingredient()
{
    return m_matched_ingredient;
}

void Allergen::set_possible_ingredients(vector<Ingredient *> possible)
{
    m_possible_ingredients = possible;
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

void Ingredient::set_done(bool done)
{
    m_done = done;
}

bool Ingredient::is_done()
{
    return m_done;
}

void Ingredient::set_matched_allergen(Allergen * matched)
{
    m_matched_allergen = matched;
}

Allergen * Ingredient::get_matched_allergen()
{
    return m_matched_allergen;
}

void Ingredient::set_possible_allergens(vector<Allergen *> possible)
{
    m_possible_allergens = possible;
}

vector<Allergen *> Ingredient::get_possible_allergens()
{
    return m_possible_allergens;
}

Food::Food(int number)
{
    m_number = number;
    m_allergens.clear();
    m_ingredients.clear();
}

Food::~Food()
{
}

int Food::get_number()
{
    return m_number;
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
        Food * food = new Food(line+1);
        
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


vector<Food *> AocDay21::get_foods_for_allergens(vector<Food *> & foods, vector<Allergen *> allergens)
{
    vector<Food *> containing;
    for (int i=0; i<foods.size(); i++)
    {
        bool has_all = true;
        for (int j=0; j<allergens.size(); j++)
        {
            if (!foods[i]->has_allergen(allergens[j]))
            {
                has_all = false;
                break;
            }
        }
        if (has_all == true)
        {
            containing.push_back(foods[i]);
        }
    }
    return containing;
}

vector<Ingredient *> AocDay21::get_common_not_done_ingredients(vector<Food *> & foods)
{
    vector<Ingredient *> found;
    vector<Ingredient *> possibilities;
    possibilities = foods[0]->get_not_done_ingredients();
    for (int i=0; i<possibilities.size(); i++)
    {
        bool in_all = true;
        for (int j=1; j<foods.size(); j++)
        {
            if (!foods[j]->has_ingredient(possibilities[i]))
            {
                in_all = false;
                break;
            }
        }
        if (in_all == true)
        {
            found.push_back(possibilities[i]);
        }
    }
    return found;
}

/* Initial plan
* Set a work_done variable to true before entering the master loop
* Master loop - done while work_done is true:
    * Set work_done variable to false
    * Loop over the list of Allergens with allergen
        * If the allergen is done, skip it
        * Get a list of all of the foods that contain the allergen.
        * Build a list of all of the not done ingredients that are common to all of the foods
        * If that list only has one member, we know that is the food that contains the allergen
            * Set the allergen's matched_ingredient to the ingredient in the list.
            * Set the ingredient's matched allergen to the allergen.
            * Set both the ingredient and the allergent to done.
            * Set the work_done variable to true
        * Else - more than one member - skip it
        * Else - zero members - throw an error and redo some logic
    * If work_done is true, start the master loop over again. There may be more we can eliminate before going to the pair-wise round
    * Loop over the list of Allergens with allergen1 - starting from position 0 to position *n-1* where *n* is the number of Allergens
        * Loop over the list of Allergens with allergen2 - starting from one after the allergen1 position, and going to position *n*
            * Get a list of all of the foods that contain both allergens
                * Build a list of all of the not done ingredients that are common to all of the foods
                    * If this list has two members, we know we have a pair-wise match
                        * Add both ingredients to both allergens' possible_ingredients lists.
                        * Add both allergens to both ingredients' possible_allergens lists.
                        * Set both ingredients and both allergens to done.
                        * Set the work_done variable to true
                    * Else - more than two members - skip it
                    * Else - zero or one members - throw and error and redo some logic
            * If work_done is true, break out of this; go back to single-elements before doing more pairs
        * If work_done is true, break out of this; go back to single-elements before doing more pairs
* At this point, all Allergens should be done. If not, more logic needs to be added to the program.
*/
void AocDay21::work_allergens(vector<Food *> & foods, vector<Allergen *> & allergens, vector<Ingredient *> & ingredients)
{
    bool work_done = true;
    while (work_done)
    {
#ifdef DEBUG_DAY21
        cout << "New Loop" << endl;
#endif
        work_done = false;
#ifdef DEBUG_DAY21
        cout << "Single Allergens check" << endl;
#endif
        for (int allergen_pos=0; allergen_pos < allergens.size(); allergen_pos++)
        {
            Allergen * allergen = allergens[allergen_pos];
            if (allergen->is_done())
            {
#ifdef DEBUG_DAY21
                cout << " Allergen " << allergen->get_name() << " is already done" << endl;
#endif
                continue;
            }
            
#ifdef DEBUG_DAY21
            cout << " Working Allergen " << allergen->get_name() << endl;
#endif
            vector<Allergen *> lookup;
            lookup.push_back(allergen);
            vector<Food *> containing_foods = get_foods_for_allergens(foods, lookup);

#ifdef DEBUG_DAY21
            cout << " found in the following foods:" << endl;
            for (int i=0; i<containing_foods.size(); i++)
            {
                cout << "  " << containing_foods[i]->get_number() << endl;
            }
#endif

            vector<Ingredient *> common_ingredients = get_common_not_done_ingredients(containing_foods);
#ifdef DEBUG_DAY21
            cout << " Those foods have the following common ingredients:" << endl;
            for (int i=0; i<common_ingredients.size(); i++)
            {
                cout << "  " << common_ingredients[i]->get_name() << endl;
            }
#endif
            
            if (common_ingredients.size() == 1)
            {
                Ingredient * ingredient = common_ingredients[0];
#ifdef DEBUG_DAY21
                cout << " Matching Allergen " << allergen->get_name() << " with Ingredient " << ingredient->get_name() << endl;
#endif
                allergen->set_matched_ingredient(ingredient);
                ingredient->set_matched_allergen(allergen);
                allergen->set_done(true);
                ingredient->set_done(true);
                work_done = true;
            }
            else if (common_ingredients.size() > 1)
            {
#ifdef DEBUG_DAY21
                cout << " Too many possibilities to resolve this allergen" << endl;
#endif
            }
            else
            {
                cout << "****CHECK LOGIC - Allergen " << allergen->get_name() << " does not find common ingredients " << endl;
            }
        }
        
        if (work_done) // work all singles before trying pairs
        {
            continue;
        }
        
#ifdef DEBUG_DAY21
        cout << "Double Allergens check" << endl;
#endif
        for (int allergen1_pos=0; allergen1_pos < allergens.size() - 1; allergen1_pos++)
        {
            Allergen * allergen1 = allergens[allergen1_pos];
            if (allergen1->is_done())
            {
#ifdef DEBUG_DAY21
                cout << " Allergen1 " << allergen1->get_name() << " is already done" << endl;
#endif
                continue;
            }
            for (int allergen2_pos=allergen1_pos+1; allergen2_pos < allergens.size(); allergen2_pos++)
            {
                Allergen * allergen2 = allergens[allergen2_pos];
                if (allergen2->is_done())
                {
#ifdef DEBUG_DAY21
                    cout << "  Allergen2 " << allergen2->get_name() << " is already done" << endl;
#endif
                    continue;
                }
            
#ifdef DEBUG_DAY21
                cout << " Working Allergens " << allergen1->get_name() << " and " << allergen2->get_name() << endl;
#endif
                vector<Allergen *> lookup;
                lookup.push_back(allergen1);
                lookup.push_back(allergen2);
                vector<Food *> containing_foods = get_foods_for_allergens(foods, lookup);
    
#ifdef DEBUG_DAY21
                cout << " found in the following foods:" << endl;
                for (int i=0; i<containing_foods.size(); i++)
                {
                    cout << "  " << containing_foods[i]->get_number() << endl;
                }
#endif
    
                vector<Ingredient *> common_ingredients = get_common_not_done_ingredients(containing_foods);
#ifdef DEBUG_DAY21
                cout << " Those foods have the following common ingredients:" << endl;
                for (int i=0; i<common_ingredients.size(); i++)
                {
                    cout << "  " << common_ingredients[i]->get_name() << endl;
                }
#endif
                
                if (common_ingredients.size() == 2)
                {
                    Ingredient * ingredient = common_ingredients[0];
#ifdef DEBUG_DAY21
                    cout << " Matching possibiliites of allergens " << allergen1->get_name() << " and " << allergen2->get_name() << " with " 
                         << common_ingredients[0]->get_name() << " and " << common_ingredients[1]->get_name() << endl;
#endif
                    allergen1->set_possible_ingredients(common_ingredients);
                    allergen2->set_possible_ingredients(common_ingredients);
                    common_ingredients[0]->set_possible_allergens(lookup);
                    common_ingredients[1]->set_possible_allergens(lookup);
                    allergen1->set_done(true);
                    allergen2->set_done(true);
                    common_ingredients[0]->set_done(true);
                    common_ingredients[1]->set_done(true);
                    work_done = true;
                }
                else if (common_ingredients.size() > 2)
                {
#ifdef DEBUG_DAY21
                    cout << " Too many possibilities to resolve this allergen" << endl;
#endif
                }
                else if (common_ingredients.size() == 1)
                {
                    cout << "****CHECK LOGIC - Allergens " << allergen1->get_name() << " and " << allergen2->get_name() << " only have one common ingredient " << common_ingredients[0]->get_name() << endl;
                }
                else
                {
                    cout << "****CHECK LOGIC - Allergens " << allergen1->get_name() << " and " << allergen2->get_name() << " do not have common ingredients" << endl;
                }
                
                if (work_done)
                {
                    break;
                }
            }
            if (work_done)
            {
                break;
            }
        }
    }
    
    for (int i=0; i<allergens.size(); i++)
    {
        if (!allergens[i]->is_done())
        {
            cout << "*****Allergen " << allergens[i]->get_name() << " was not completed. Time to add more logic!!" << endl;
        }
    }
}

string AocDay21::part1(string filename, vector<string> extra_args)
{
    vector<Food *> foods;
    vector<Allergen *> allergens;
    vector<Ingredient *> ingredients;
    
    int total_not_allergen=0;
    
    parse_input(filename, foods, allergens, ingredients);
    
    work_allergens(foods, allergens, ingredients);
    
    for (int i=0; i<foods.size(); i++)
    {
        total_not_allergen+=foods[i]->get_not_done_ingredients().size();
    }
    
    cleanup(foods, allergens, ingredients);
    
    ostringstream out;
    out << total_not_allergen;
    return out.str();
}

