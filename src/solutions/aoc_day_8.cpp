#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "aoc_day_8.h"
#include "file_utils.h"

using namespace std;

Computer::Computer()
{
    m_instruction_pointer = 0;
    m_accumulator = 0;
    m_operations = Operations::get_instance();
}

Computer::~Computer()
{
}

long Computer::get_instruction_pointer()
{
    return m_instruction_pointer;
}

void Computer::set_instruction_pointer(long value)
{
    m_instruction_pointer = value;
}

void Computer::adjust_instruction_pointer(long adjustment)
{
    m_instruction_pointer += adjustment;
}

long Computer::get_accumulator()
{
    return m_accumulator;
}

void Computer::set_accumulator(long value)
{
    m_accumulator = value;
}

void Computer::adjust_accumulator(long adjustment)
{
    m_accumulator += adjustment;
}

void Computer::run_instruction(Instruction * instruction)
{
    Operation * operation = m_operations->get_operation(instruction->get_operation());
    if (operation == NULL)
    {
        cerr << "*****INVALID OPERATION " << instruction->get_operation() << " SPECIFIED*****" << endl;
        return;
    }
    operation->run_operation(instruction, this);
}

Instruction::Instruction(string operation, long argument)
{
    m_operation = operation;
    m_argument = argument;
    m_run = false;
}

Instruction::~Instruction()
{
}

string Instruction::get_operation()
{
    return m_operation;
}

long Instruction::get_argument()
{
    return m_argument;
}

bool Instruction::is_run()
{
    return m_run;
}

void Instruction::set_run(bool run)
{
    m_run = run;
}

Program::Program()
{
}

Program::~Program()
{
    for (int i=0; i<m_instructions.size(); i++)
    {
        delete m_instructions[i];
    }
    m_instructions.clear();
}

void Program::add_instruction(Instruction * instruction)
{
    cout << "Program line " << m_instructions.size() << " is " << instruction->get_operation() << " " << instruction->get_argument() << endl;
    m_instructions.push_back(instruction);
}

Instruction * Program::get_instruction(long line)
{
    if (line < 0 || line >= m_instructions.size())
    {
        cerr << "***ATTEMPTING TO ACCESS LINE " << line << ". VALID LINES ARE 0 - " << m_instructions.size() - 1 << "*****" << endl;
        return NULL;
    }
    return m_instructions[line];
}

Operation::Operation(string operation)
{
    m_operation = operation;
}

Operation::~Operation()
{
}

AccOperation::AccOperation():Operation(ACC)
{
}

AccOperation::~AccOperation()
{
}

// acc: increment the Computer's accumulator by the argument and the instruction pointer by 1.  
void AccOperation::run_operation(Instruction * instruction, Computer * computer)
{
    computer->adjust_accumulator(instruction->get_argument());
    computer->adjust_instruction_pointer(1);
}

JmpOperation::JmpOperation():Operation(ACC)
{
}

JmpOperation::~JmpOperation()
{
}

// jmp: increment the Computer's instruction_pointer by the argument.  
void JmpOperation::run_operation(Instruction * instruction, Computer * computer)
{
    computer->adjust_instruction_pointer(instruction->get_argument());
}

NopOperation::NopOperation():Operation(ACC)
{
}

NopOperation::~NopOperation()
{
}

// nop: increment the Computer's insturction_pointer by 1.  
void NopOperation::run_operation(Instruction * instruction, Computer * computer)
{
    computer->adjust_instruction_pointer(1);
}

Operations * Operations::m_instance = NULL;

Operations::Operations()
{
    m_operations[ACC]=new AccOperation();
    m_operations[JMP]=new JmpOperation();
    m_operations[NOP]=new NopOperation();
}

Operations * Operations::get_instance()
{
    if (m_instance == NULL)
    {
        m_instance = new Operations();
    }
    return m_instance;
}

Operations::~Operations()
{
    for (map<string, Operation *>::iterator del_iter = m_operations.begin(); del_iter != m_operations.end(); ++del_iter)
    {
        delete del_iter->second;
    }
    m_operations.clear();
}

Operation * Operations::get_operation(string name)
{
    return m_operations[name];
}

AocDay8::AocDay8():AocDay(8)
{
}

AocDay8::~AocDay8()
{
}

void AocDay8::load_input_to_program(string filename, Program * program)
{
    FileUtils fileutils;
    vector<vector<string>> split_lines;
    if (!fileutils.read_as_list_of_split_strings(filename, split_lines, ' ', '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return;
    }
    for (vector<vector<string>>::iterator iter = split_lines.begin(); iter != split_lines.end(); ++iter)
    {
        vector<string> line = *iter;
        Instruction * inst = new Instruction(line[0], strtol(line[1].c_str(), NULL, 10));
        program->add_instruction(inst);
    }
    return;
};

string AocDay8::part1(string filename, vector<string> extra_args)
{
    Program program;
    Computer computer;
    load_input_to_program(filename, &program);
    
    ostringstream out;
    out << computer.get_accumulator();
    return out.str();
}
