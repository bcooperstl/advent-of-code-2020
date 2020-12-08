# Day 7, Part 2 #

## Requirements ##

There are a set of baggage rules as defined in the input format that specify specific color bags that contain other color bags.

For example: `light red bags contain 1 bright white bag, 2 muted yellow bags.`

These can be nested, so that a red bag can be in a green bag can be in a blue bag.

Going through the nesting of rules, we need to find how many bags will be contained in a `shiny gold` bag.

### Input Format ###

A list of rules, one rule per line.

Some sample rules:
* `light red bags contain 1 bright white bag, 2 muted yellow bags.`
* `bright white bags contain 1 shiny gold bag.`
* `faded blue bags contain no other bags.`

### Output Format ###

Integer - the number of bag colors that contain the shiny gold bag

## Test Cases ##

### Given Test Cases ###

There are two examples given:
1) [test example 1](../data/test_cases/day7_test1.txt) given. 
2) [test example 2](../data/test_cases/day7_test2.txt) given. 

I think that will be enough.

### Custom Test Cases ###


## Approach ##

### Parsing the Input ###

This is the same as [Day 7, Part 1](day7_part1.md).

To parse the input, I will use a series of regular expressions.

For the simple case, a bag containing no other bags, I can use a regex "`(.*) bags contain no other bags\.`"  This will let me extract the color in the first group and I don't need anything else.

For a more comoplicated case, a bag that contains other bags, I can start with the regex "`(.*) bags contain (.*)`" This will let me extract the target color in the first group and the dependencies in the second group.  
I can then run the second group through a regex "`(\d+) (.*) bags?[,\.] ?`". This will match the quantity in the first group, the color in the second group, the word bag or bags, the comma or period, and an optional space at the end. This should find multiple matches for if there are more than 1 contained bags specified.

### Storing the data ###

This is the same as [Day 7, Part 1](day7_part1.md).

I will create a BagContents class to store what a Bag contains. It will have a quantity and a pointer to a Bag class for the child.

I will create a Bag class to contain each bag. It will have the color of the bag. It will have a list of Bags that can contain it, called contained_by. It will have a list of contents that contain the direct contents of the bag. There will be a processed flag that will be use when evaluating the solution.
*I am Adding a number_of_contained_bags element to the Bag class for Part 2*

There will also be a lookup map going from a string to a Bag, so that I can quickly find the Bag for a color.

### Determining the answer ###

* Use the lookup table to find the shiny gold bag, and store this at the beginning of the to_process list.
* Loop over the to_process list, adding all of the Bags from that Bag's contents list to the to_process list. There will be duplicates, and that is ok. Eventually, the bags will have empty contents lists, so this will terminate.

* Loop over the to_process list in reverse. 
* If the Bag has already been processed, do nothing
* If not, calculate the number of contained bags by multiplying each quantity by the corresponding Bag's (number of contained bags + 1), and summing those results. The +1 is for the bag itself

* The shiny gold Bag's number of contained bags is the final result.

## Things I learned ##

I thought it was a neat way to build the list forward through the list with duplicates, then just not process them going backwards if they had already been processed. This assured that all children will be processed before the parents.
