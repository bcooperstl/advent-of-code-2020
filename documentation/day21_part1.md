# Day 21, Parts 1 and 2 #

## Requirements ##

Given a list of foods with their ingredients and a partial list of their allergens, use logic to determine which ingredients can't possibly contain any allergens.

There is only one ingredient that contains an allergen. The allergen may or may not be marked in all foods, so the non-presense of the allergen does not invalidate the ingredient.

For part 1, Return a count of the number of ingredients in all the foods that can't possibly contain an allergen. Note that if an ingredient is in multiple foods, it is counted multiple times.

For part 2, Order the allergens alphabetically, and then make a comma-separated list of the ingredients that go with those allergens.

### Input Format ###

List of foods.
Each foods contains one or more ingredients, followed by one or more allergens.

Example input line:    
`mxmxvkd kfcds sqjhc nhms (contains dairy, fish)`

By inspection, I found that the most allergents a single food contains is 3.

### Output Format ###

Part 1: Integer -  the number of times ingredients appear that cannot possibly contain allergens.

Part 2: String - comma separted list of the ingredient names that go with the allergens.

## Test Cases ##

### Given Test Cases ###

There is [one](../data/test_cases/day21_test1.txt) given test case.

### Custom Test Cases ###

I will make a [second](../data/test_cases/day21_test2.txt) test case to check eliminating ingredients when the exact allergen can't be determined, but knowing it has one of two can be.

## Approach ##

### Parsing the Data ###

I will use the existing split into vector<vector<string>> function to split the data.

For each line that is parsed, I will look until I get the string `(contains`.  
Items before that string are ingredients.  
Items after that string are allergens, but the final character (either a `,` or `)`) needs to be stripped off.

### Internal Representation ###

There will be an Allergen class with:
* name
* matched Ingredient
* possible Ingredients
* boolean done to indicate if this Allergen is done processing

There will be an Ingredient class with:
* name
* matched Allergen
* possible Allergens
* boolean done to indicate that this Ingredient is done processing

There will be a Food class with:
* list of Allergens
* list of Ingredients
* get_not_done_allergens function - returns all Allergens that are do not have done set to true.
* get_not_done_ingredients function - returns all Ingredients that do not have done set to true.
* has_allergen function - returns true/false for if it has that Allergen
* has_ingredient function - returns true/false for if it has that Ingredient

There will be lists of all allergens, ingredients, and foods.

### Working through the logic ###

I will be programming a lot of process-of-elimination logic in order to assign the allergens to ingredients. There may need to be multiple iterations of this.

I think I only have to program this to pair-wise logic. Consider foods A and B both have ingredients foo and bar, and both have allergens nuts and milk. We don't know whether foo has nuts or milk, but we know that foo has one of them and bar has the other one.

I will do the first pass of single-allergen identification first, and then the pair-wise identification.

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
        * If the allergen is done, skip it
        * Loop over the list of Allergens with allergen2 - starting from one after the allergen1 position, and going to position *n*
            * If the allergen is done, skip it
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

### Updates for Part 2 ###

All I need to do for part 2 is sort the allergen list alphabetically, and then create a comma-separated list of the ingredients that correspond to them.

## Things I learned ##

Thinking though and developing the logic for the algorithm, especially to identify the matched-pair scenario.

Well, it turns out that I didn't even need to develop the double-match category. Oh well, it was fun to do.


