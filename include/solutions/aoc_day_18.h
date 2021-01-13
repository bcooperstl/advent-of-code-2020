#ifndef __AOC_DAY_18__
#define __AOC_DAY_18__

#include "aoc_day.h"

#define OPERATION_ASSIGN 0
#define OPERATION_ADD 1
#define OPERATION_MULTIPLY 2
#define VALUE_LITERAL 3
#define VALUE_EXPRESSION 4

class Token
{
    public:
        virtual void display() = 0;
        virtual int get_type() = 0;
};

class Day18Operation : public Token
{
    public:
        virtual long perform(long left, long right) = 0;
};

class AddOperation : public Day18Operation
{
    public:
        AddOperation();
        ~AddOperation();
        void display();
        long perform(long left, long right);
        int get_type();
};

class MultiplyOperation : public Day18Operation
{
    public:
        MultiplyOperation();
        ~MultiplyOperation();
        void display();
        long perform(long left, long right);
        int get_type();
};    

class Value : public Token
{
    public:
        virtual long get_value() = 0;
};

class LiteralValue : public Value
{
    private:
        long m_value;
    public:
        LiteralValue(long value);
        ~LiteralValue();
        void display();
        long get_value();
        int get_type();
};

class Expression : public Value
{
    private:
        vector<Token *> m_tokens;
        int get_first_token_index(int type);
    public:
        Expression();
        ~Expression();
        void display();
        void add_token(Token * token);
        long get_value();
        int get_type();
};
        
class AocDay18 : public AocDay
{
    private:
        vector<string> read_input(string filename);
        Expression * tokenize_expression(string input);
        long eval_expression(string::iterator & pos, string::iterator & end, int level);
    public:
        AocDay18();
        ~AocDay18();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};


#endif
