# Day 18, Part 2 #

## Requirements ##

This problem is going to perform mathematical calcaultions, but with the caveat that addition has a high precedence in the order of operations than multiplication.
There are also parentheses like normal math that indicate expressions that must be solved first, but with the different order-of-operations for addition and multiplication.

The program is to take a list of expressions (one per line), compute their solution, and then return the sum of those solutions.

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

Unlike [part 1](day18_part1.md), I think I'm going to have to tokenize this out in order to process addition before multiplication.

### Defined Structues ###

#### Token ####
There will be a pure virtual `token` class, that will be used in tokenizing the equation.

#### Operations ####
There will be a base virtual operation class which inherits from token. It has a virtual `long perform(long left, long right)` function that performs its operation.
There will be a `add_operation` and `multiply_operation` subclasses that correspond to the corresonding operations.

#### Values ####
There will be a base virtual value class which inherits from token. It has a virtual `get_value()` function.

There will be a literal_value subclass. Its get_value() returns the numeric value that is passed in at construction time. This is used for the 0-9 values in the input expressions.  

There will be a expression subclass. Its get_value() returns computed value from the expression. It will contain a vector<Token> for the contianing tokens.
It will contain an add_token method to add a token to its internal vector


### Tokenizing an expression ###

* Define a `vector<Expression *> expression_stack` variable to hold a stack of expressions. This is used for nested parentheses.
* Allocate an `Expression * parent` to stand for the equation as a whole
* Append `parent` onto `expression_stack`
* Set `Expression * current_expression` to `parent`
* Loop over all of the characters in the input string with current
    * If current is a space
        * Drop it and continue with the next iteration of the loop
    * Else if current is a digit between 0 and 9
        * Create a literal_value object from this digit, and append this to `current_expression`
    * Else if current is an open parenthesis `(`
        * Create an expression next.
        * Append next to current_expression.
        * Append current_expression onto the expression_stack
        * Set current_expression to next
    * Else if current is a closed parenthesis ')'
        * Set current_expression to expression_stack.back()
        * pop the back element off of expression_stack
    * Else if current is a plus sign '+'
        * Append a add_operation to the current_expression
    * Else if current is a multiplication sign '*'
        * Append a multiply_operation to the current_expression
    * Else
        * Throw an error; Something is wrong here and I need to figure it out
* Return parent

### Evaluating an expression ###
We need to do all of the additions first, and all of the multiplications second.

We know that we will have a value, then an operation, then a value, etc.

* While an addition operations to be done is found at position i
    * Call the add_operations(process(token[i-1].get_value(),token[i+1].get_value()) and store in result.
    * Remove the i-1, i and i+1 tokens from the vector. 
    * Create a literal_value from the result and store it at position i-1
* While there are multiplications to be done
    * Call the add_operations(process(token[i-1].get_value(),token[i+1].get_value()) and store in result.
    * Remove the i-1, i and i+1 tokens from the vector. 
    * Create a literal_value from the result and store it at position i-1
* Return the one remaining literal_value as the result

### Main Loop ###
* Read in the list of expressions to a string
* Init a sum variable to 0
* For each expression
    * Tokenize the input string into an Expression
    * Evaluate the expression
    * Inrement the sum by the result of the previous call
* Return the sum

## Things I learned ##

Wow, this algorithm was very interesting to come up with for how to tokenize and then evaluate the expressions. I hope the recursive nature of it is correct.


### Alternate Approaches ###

If it wasn't so long ago, I'd probably try to code this up using yacc and bison. However, it's been about 18 years since I last used those, and I don't want to spend two weeks re-learning them.
