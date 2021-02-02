#ifndef __AOC_DAY_21__
#define __AOC_DAY_21__

#include <string>
#include <vector>

#include "aoc_day.h"

using namespace std;

class Allergen;
class Ingredient;

class Allergen
{
    private:
        string m_name;
        Ingredient * m_matched_ingredient;
        vector<Ingredient *> m_possible_ingredients;
        bool m_done;
    public:
        Allergen(string name)
        ~Allergen();
        string get_name();
        bool is_done();
        Ingredient * get_matched_ingredient();
        vector<Ingredient *> get_possible_ingredients();
};
      
class Ingredient
{
    private:
        string m_name;
        Allergen * m_matched_allergen;
        vector<Allergen *> m_possible_allergens;
        bool m_done;
    public:
        Ingredient(string name)
        ~Ingredient();
        string get_name();
        bool is_done();
        Allergen * get_matched_allergen();
        vector<Allergen *> get_possible_allergens();
};

class Food
{
    private:
        vector<Allergen *> m_allergens;
        vector<Ingredient *> m_ingredients;
    public:
        Food();
        ~Food();
        void add_allergen(Allergen * allergen);
        vector<Allergen *> get_all_allergens();
        vector<Allergen *> get_not_done_allergens();
        bool has_allergen(Allergen * allergen);
        void add_ingredient(Ingredient * ingredient);
        vector<Ingredient *> get_all_ingredients();
        vector<Ingredient *> get_not_done_ingredients();
        bool has_ingredient(Ingredient * ingredient);
};

class AocDay21 : public AocDay
{
    private:
        void parse_input(string filename, vector<Food *> & foods, vector<Allergen *> & allergens, vector<Ingredient *> & ingredients);
        void cleanup(vector<Food *> & foods, vector<Allergen *> & allergens, vector<Ingredient *> & ingredients);
    public:
        AocDay21();
        ~AocDay21();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};


#endif
