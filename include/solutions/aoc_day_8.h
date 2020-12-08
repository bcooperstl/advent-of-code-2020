#ifndef __AOC_DAY_8__
#define __AOC_DAY_8__

#include <string>
#include <vector>
#include <map>

#include "aoc_day.h"

using namespace std;

class Operations;
class Instruction;

class Computer
{
    private:
        long m_instruction_pointer;
        long m_accumulator;
        Operations * m_operations;
    public:
        Computer();
        ~Computer();
        long get_instruction_pointer();
        void set_instruction_pointer(long value);
        void adjust_instruction_pointer(long adjustment);
        long get_accumulator();
        void set_accumulator(long value);
        void adjust_accumulator(long adjustment);
        void run_instruction(Instruction * instruction);
};

class Instruction
{
    private:
        string m_operation;
        long m_argument;
        bool m_run;
    public:
        Instruction(string operation, long argument);
        ~Instruction();
        bool is_run();
        void set_run(bool run);
};

class Program
{
    private:
        vector<Instruction *> m_instructions;
    public:
        Program();
        ~Program();
        void add_instruction(Instruction * instruction);
        Instruction * get_instruction(long line);
};

class Operation        
{
    protected:
        string m_operation;
    public:
        Operation(string operation);
        ~Operation();
        virtual void run_operation(Instruction * instruction, Computer * computer) = 0;
};

class AccOperation : public Operation
{
    void run_operation(Instruction * instruction, Computer * computer);
};

class JmpOperation : public Operation
{
    void run_operation(Instruction * instruction, Computer * computer);
};

class NopOperation : public Operation
{
    void run_operation(Instruction * instruction, Computer * computer);
};

class Operations
{
    private:
        static Operations * m_instance;
        Operations();
    public:
        static Operations * get_instance();
        ~Operations();
        Operation * lookup_operations(string name);
};

class AocDay8 : public AocDay
{
    private:
    public:
        AocDay8();
        ~AocDay8();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};


#endif
