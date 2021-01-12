# Day 18, Part 1 #

## Requirements ##

This problem is going to perform mathematical calcaultions, but with the caveat that addition and multiplication are the same order-of-operations precedence.  
There are also parentheses like normal math that indicate expressions that must be solved first, but with the different order-of-operations for addition and multiplication.

The program is to take a list of expressions (one per line), compute their solution, and then return the sum of those solutions.
Given a list of 

### Input Format ###

List of expressions, one per line. Expressions all contain single digits, addition signs `+`, multiplication signs `*` and prentheses `(` and `)`. There are also some spaces in the lines.

### Output Format ###

Number - the sum of the solutions as described above.

## Test Cases ##

### Given Test Cases ###

There are six individual expressions given. I will create a test case for each of those: 
[1](../test_cases/day18_test1.txt), 
[2](../test_cases/day18_test2.txt), 
[3](../test_cases/day18_test3.txt), 
[4](../test_cases/day18_test4.txt), 
[5](../test_cases/day18_test5.txt), 
[6](../test_cases/day18_test6.txt).

### Custom Test Cases ###

I will create another [test case](../test_cases/day18_test7.txt) with all 6 of the prior single expressions, to test the summing.

## Approach ##

I'm going to handle each line as its own expression, iterating over it with an iterator named pos.

I will define three constants for the operations: OPERATION_ADD, OPERATION_MULTIPLY, and OPERATION_ASSIGN (for assigning the first operand)

### Parsing and evaluating an expression ###
The function signature will look like `long eval_expression(string::iterator & pos, string::iterator & end, int level);`
* Define a long variable `total = 0`
* Define a variable `next_operation` = OPERATION_ASSIGN
* Define a variable `next_operand = 0`
* While pos is not equal to end
    * Set char current to the character pointed to by pos and increment pos
    * If current is a space
        * Continue with the next iteration of the loop
    * Else if current is a digit between 0 and 9
        * Set next_operand to the integer value of the digit
    * Else if current is an open parenthesis `(`
        * Recursively call eval_expression with (pos, end, level+1), storing the result in next_operand
    * Else if current is a closed parenthesis ')'
        * Return total
    * Else if current is a plus sign '+'
        * Set next_operation = OPERATION_ADD
        * Continue with the next iteration of the loop
    * Else if current is a multiplication sign '*'
        * Set next_operation = OPERATION_MULTIPLY
        * Continue with the next iteration of the loop
    * Else
        * Throw an error; Something is wrong here and I need to figure it out
    * If next_operation is OPERATION_ASSIGN
        * Set total equal to first_operand
    * Else if next_operation is OPERATION_ADD
        * Set total = total + next_operand
    * Else if next_operation is OPERATION_MULTIPLY
        * Set total = total * next_operand
* Return total // this should only happen when we reach the end of the input string

### Main Loop ###
* Read in the list of expressions to a string
* Init a sum variable to 0
* For each expression
    * Call the eval_expressions with expression.begin(), expression.end(), 0). Note that the level is only used for debugging info
    * Inrement the sum by the result of the previous call
* Return the sum

## Things I learned ##

Going to be passing around a string iterator for this solution. Need to pass it by reference so that it will come back in the correct location.
With this, had to define variables to store the iterators instead of just taking the return values from the .begin() and .end() calls. Makes sense.

### Alternate Approaches ###

If it wasn't so long ago, I'd probably try to code this up using yacc and bison. However, it's been about 18 years since I last used those, and I don't want to spend two weeks re-learning them.
