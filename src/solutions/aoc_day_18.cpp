#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iterator>

#include "aoc_day_18.h"
#include "file_utils.h"

using namespace std;

#define DEBUG_MATH

#define TOKEN_NOT_FOUND -1

AocDay18::AocDay18():AocDay(18)
{
}

AocDay18::~AocDay18()
{
}

vector<string> AocDay18::read_input(string filename)
{
    FileUtils fileutils;
    vector<string> data;
    if (!fileutils.read_as_list_of_strings(filename, data))
    {
        cerr << "Error reading in the data from " << filename << endl;
    }
    return data;
}


/* Intial plan
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
*/
long AocDay18::eval_expression(string::iterator & pos, string::iterator & end, int level)
{
#ifdef DEBUG_MATH
    cout << "Evaluating at level " << level << endl;
#endif
    long total = 0;
    int next_operation = OPERATION_ASSIGN;
    long next_operand = 0;
    while (pos != end)
    {
        char current = *pos;
        ++pos;
#ifdef DEBUG_MATH
        cout << " Evaluating [" << current << "]. ";
#endif
        if (current == ' ')
        {
#ifdef DEBUG_MATH
            cout << "Skipping a space" << endl;
#endif
            continue;
        }
        else if (current >= '0' && current <= '9')
        {
#ifdef DEBUG_MATH
            cout << "Setting next_operand to " << current-'0' << endl;
#endif
            next_operand = current-'0';
        }
        else if (current == '(')
        {
#ifdef DEBUG_MATH
            cout << "Recursively calling to evaluate the next level" << endl;
#endif
            next_operand = eval_expression(pos, end, level+1);
#ifdef DEBUG_MATH
            cout << " At level " << level << " received " << next_operand << " from the recursive call" << endl;
#endif
        }
        else if (current == ')')
        {
#ifdef DEBUG_MATH
            cout << "Returning the current total of " << total << endl;
#endif
            return total;
        }
        else if (current == '+')
        {
#ifdef DEBUG_MATH
            cout << "Setting next operation to ADD" << endl;
#endif
            next_operation = OPERATION_ADD;
            continue;
        }
        else if (current == '*')
        {
#ifdef DEBUG_MATH
            cout << "Setting next operation to MULTIPLY" << endl;
#endif
            next_operation = OPERATION_MULTIPLY;
            continue;
        }
        else
        {
            cout << "*****INVALID INPUT******" << endl;
            return 0;
        }
        
        // Will only get this far when an operand has been set. Need to handle it.
        if (next_operation == OPERATION_ASSIGN)
        {
            total = next_operand;
#ifdef DEBUG_MATH
            cout << " Assigning total = " << next_operand << endl;
#endif
        }
        else if (next_operation == OPERATION_ADD)
        {
            total += next_operand;
#ifdef DEBUG_MATH
            cout << " Adding " << next_operand << " to total to get " << total << endl;
#endif
        }
        else if (next_operation == OPERATION_MULTIPLY)
        {
            total *= next_operand;
#ifdef DEBUG_MATH
            cout << " Multiplying " << next_operand << " to total to get " << total << endl;
#endif
        }
    }
#ifdef DEBUG_MATH
    cout << " Got the end of the string. Returning total " << total << endl;
#endif
    return total;
}

string AocDay18::part1(string filename, vector<string> extra_args)
{
    vector<string> expressions = read_input(filename);
    long sum = 0;
    for (vector<string>::iterator expression_iter = expressions.begin(); expression_iter != expressions.end(); ++expression_iter)
    {
        string expression = *expression_iter;
        cout << "Evaluating expression " << expression << endl;
        string::iterator pos = expression.begin();
        string::iterator end = expression.end();
        long value = eval_expression(pos, end, 0);
        cout << expression << " = " << value;
        sum+=value;
    }
    ostringstream out;
    out << sum;
    return out.str();
}

AddOperation::AddOperation()
{
}

AddOperation::~AddOperation()
{
}

void AddOperation::display()
{
    cout << " + ";
}

long AddOperation::perform(long left, long right)
{
    return left + right;
}

int AddOperation::get_type()
{
    return OPERATION_ADD;
}

MultiplyOperation::MultiplyOperation()
{
}

MultiplyOperation::~MultiplyOperation()
{
}

void MultiplyOperation::display()
{
    cout << " * ";
}

long MultiplyOperation::perform(long left, long right)
{
    return left * right;
}

int MultiplyOperation::get_type()
{
    return OPERATION_MULTIPLY;
}

LiteralValue::LiteralValue(long value)
{
    m_value = value;
}

LiteralValue::~LiteralValue()
{
}

void LiteralValue::display()
{
    cout << m_value;
}

long LiteralValue::get_value()
{
    return m_value;
}

int LiteralValue::get_type()
{
    return VALUE_LITERAL;
}

Expression::Expression()
{
}

Expression::~Expression()
{
    for (int i=0; i<m_tokens.size(); i++)
    {
        delete m_tokens[i];
    }
}

void Expression::display()
{
    cout << " ( ";
    for (int i=0; i<m_tokens.size(); i++)
    {
        m_tokens[i]->display();
    }
    cout << " ) ";
}

void Expression::add_token(Token * token)
{
    m_tokens.push_back(token);
}

// Return the index to the first token of the given type. Returns -1 for not found
int Expression::get_first_token_index(int type)
{
    for (int i=0; i<m_tokens.size(); i++)
    {
        if (m_tokens[i]->get_type() == type)
        {
            return i;
        }
    }
    
    return TOKEN_NOT_FOUND;
}

/* Planned approach
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
*/

long Expression::get_value()
{
    cout << "Evaluating expression ";
    display();
    cout << endl;
    int position = get_first_token_index(OPERATION_ADD);
    while (position != TOKEN_NOT_FOUND)
    {
        cout << "  Performing addition of position=" << position-1 << " contents=[";
        m_tokens[position-1]->display();
        cout << "] and position=" << position+1 << " contents=[";
        m_tokens[position+1]->display();
        cout << "]" << endl;
        long result = ((Day18Operation *)m_tokens[position])->perform(((Value *)m_tokens[position-1])->get_value(), ((Value *)m_tokens[position+1])->get_value());
        delete m_tokens[position-1];
        delete m_tokens[position];
        delete m_tokens[position+1];
        m_tokens.erase(m_tokens.begin()+(position-1), m_tokens.begin()+(position+1)+1); // remove from i-1 to i+1. Need to add the last "+1" because erase doesn't include that last element
        m_tokens.insert(m_tokens.begin()+(position-1), new LiteralValue(result));
        position = get_first_token_index(OPERATION_ADD);
        display();
    }
    position = get_first_token_index(OPERATION_MULTIPLY);
    while (position != TOKEN_NOT_FOUND)
    {
        cout << "  Performing multiplication of position=" << position-1 << " contents=[";
        m_tokens[position-1]->display();
        cout << "] and position=" << position+1 << " contents=[";
        m_tokens[position+1]->display();
        cout << "]" << endl;
        long result = ((Day18Operation *)m_tokens[position])->perform(((Value *)m_tokens[position-1])->get_value(), ((Value *)m_tokens[position+1])->get_value());
        delete m_tokens[position-1];
        delete m_tokens[position];
        delete m_tokens[position+1];
        m_tokens.erase(m_tokens.begin()+(position-1), m_tokens.begin()+(position+1)+1); // remove from i-1 to i+1. Need to add the last "+1" because erase doesn't include that last element
        m_tokens.insert(m_tokens.begin()+(position-1), new LiteralValue(result));
        position = get_first_token_index(OPERATION_MULTIPLY);
        display();
    }
    if (m_tokens.size() != 1)
    {
        cout << "SOMETHING WRONG HERE - SHOULD ONLY HAVE ONE TOKEN WHEN RETURNING" << endl;
    }
    return ((Value *)m_tokens[0])->get_value();
}

int Expression::get_type()
{
    return VALUE_EXPRESSION;
}
