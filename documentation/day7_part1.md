# Day 7, Part 1 #

## Requirements ##

There are a set of baggage rules as defined in the input format that specify specific color bags that contain other color bags.

For example: `light red bags contain 1 bright white bag, 2 muted yellow bags.`

These can be nested, so that a red bag can be in a green bag can be in a blue bag.

Going through the nesting of rules, we need to find how many bag colors can eventually contain a `shiny gold` bag.

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

There is one [test example](../data/test_cases/day7_test1.txt) given. I think that will be enough.

### Custom Test Cases ###


## Approach ##

### Parsing the Input ###

To parse the input, I will use a series of regular expressions.

For the simple case, a bag containing no other bags, I can use a regex "`(.*) bags contain no other bags\.`"  This will let me extract the color in the first group and I don't need anything else.

For a more comoplicated case, a bag that contains other bags, I can start with the regex "`(.*) bags contain (.*)`" This will let me extract the target color in the first group and the dependencies in the second group.  
I can then run the second group through a regex "`(\d+) (.*) bags?[,\.] ?`". This will match the quantity in the first group, the color in the second group, the word bag or bags, the comma or period, and an optional space at the end. This should find multiple matches for if there are more than 1 contained bags specified.

### Storing the data ###

I will create a BagContents class to store what a Bag contains. It will have a quantity and a pointer to a Bag class for the child.

I will create a Bag class to contain each bag. It will have the color of the bag. It will have a list of Bags that can contain it, called contained_by. It will have a list of contents that contain the direct contents of the bag. There will be a processed flag that will be use when evaluating the solution.

There will also be a lookup map going from a string to a Bag, so that I can quickly find the Bag for a color.

### Determining the answer ###

Use the lookup table find the shiny gold Bag. From this, go through each member of the contained_by list. Mark them processed and add them to a list of container bags.  
Iterate through this container bags list, doing the same lookup and add to the end of list, but only if the looked up bag has not been processed.  
Repeat until every element in the list has been done.  
The length of this list is the result.

## Things I learned ##

More working with the c++ regex library. Getting multiple matches out of an input. Use of the regex_token_iterator to get portions of submatches
