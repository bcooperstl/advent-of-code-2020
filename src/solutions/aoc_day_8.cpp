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

void Instruction::set_operation(string operation)
{
    m_operation = operation;
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

void Program::set_all_not_run()
{
    for (int i=0; i<m_instructions.size(); i++)
    {
        m_instructions[i]->set_run(false);
    }
}

long Program::get_instruction_count()
{
    return m_instructions.size();
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

/* Planned approach
* Create a Computer, which will in turn grab the Operations singleton, which will result in the 3 derived Operations being created
* Create a Program from the input file
* Loop through the Program
    * Get the next instruction to Execute from the Computer's instruction pointer.
    * If that insturction has its run flag as true, we are done
        * Store the accumulator value from the Computer to be returned and discontinue the loop.
    * Call the Computer's run_instruction method, passing the Instruction. The Computer will fetch the Operation and call its run_operation method to do the processing.
    * Mark the instruction as ran
* Return the stored accumulator value.
*/
string AocDay8::part1(string filename, vector<string> extra_args)
{
    Program program;
    Computer computer;
    load_input_to_program(filename, &program);
    
    while (1)
    {
        cout << "Next instruction to run is " << computer.get_instruction_pointer() << endl;
        Instruction * next = program.get_instruction(computer.get_instruction_pointer());
        if (next->is_run())
        {
            cout << " Insturction has already been run. Stopping" << endl;
            break;
        }
        computer.run_instruction(next);
        next->set_run(true);
    }
    
    ostringstream out;
    out << computer.get_accumulator();
    return out.str();
}


/* Planned approach
* Create a Computer, which will in turn grab the Operations singleton, which will result in the 3 derived Operations being created
* Create a Program from the input file
* Loop through each line of the program, while success is false
    * If the line is acc, skip this iteration of the loop
    * Initialize the computer by setting the instruction pointer and accumulator to 0.
    * Initialize the program by setting the set_all_not_run method.
    * Initialize a success indicator to false.
    * Perform the nop/jmp flip for this line.
    * Loop through the Program
        * Get the next instruction number to Execute from the Computer's instruction pointer.
        * If this instruction number equals the program's line count (i.e. it would be the line following the last one)
            * We have a successful run. Set success to true, and break out of the loop.
        * If the next instruction number is less than 0 or more than the line count + 1, break; we are way out of range.
        * Get the instruction that corresponds to the instrution number.
        * If that insturction has its run flag as true, break; we have an infite loop with a repeated instruction.
        * Call the Computer's run_instruction method, passing the Instruction. The Computer will fetch the Operation and call its run_operation method to do the processing.
        * Mark the instruction as ran
    * Perform the nop/jmp flip back for this line so it is like it originally was for the next iteration.
    * If success indicator is true, break out of the parent loop. We're done.
* Return the computer's accumulator value.
*/

string AocDay8::part2(string filename, vector<string> extra_args)
{
    Program program;
    Computer computer;
    load_input_to_program(filename, &program);
    
    long num_lines = program.get_instruction_count();
    
    for (int i=0; i<num_lines; i++)
    {
        Instruction * change_instruction = program.get_instruction(i);
        // skip if acc
        if (change_instruction->get_operation() == ACC)
        {
            continue;
        }
        
        computer.set_instruction_pointer(0);
        computer.set_accumulator(0);
        program.set_all_not_run();
        bool success = false;
        
        cout << "Checking by swapping instruction " << i << endl;
        // swap the operation
        if (change_instruction->get_operation() == JMP)
        {
            cout << " Swapping from JMP to NOP" << endl;
            change_instruction->set_operation(NOP);
        }
        else
        {
            cout << " Swapping from NOP to JMP" << endl;
            change_instruction->set_operation(JMP);
        }
        
        while (1)
        {
            long instruction_number = computer.get_instruction_pointer();
            if (instruction_number == num_lines)
            {
                cout << " Next instruction to run is " << instruction_number << " which is one past the end. SUCCESS!!" << endl;
                success = true;
                break;
            }
            if (instruction_number < 0 || instruction_number > num_lines)
            {
                cout << " Next instruction to run is out of range. Got " << instruction_number << " for range 0 - " << num_lines - 1 << endl;
                break;
            }
            Instruction * next = program.get_instruction(computer.get_instruction_pointer());
            if (next->is_run())
            {
                cout << " Insturction " << instruction_number << " has already been run. Infinte loop with this modification" << endl;
                break;
            }
            computer.run_instruction(next);
            next->set_run(true);
        }
        // swap back
        if (change_instruction->get_operation() == NOP)
        {
            cout << " Swapping back to JMP" << endl;
            change_instruction->set_operation(JMP);
        }
        else
        {
            cout << " Swapping back to NOP" << endl;
            change_instruction->set_operation(NOP);
        }
        if (success)
        {
            cout << " BREAKING ON SUCCESS" << endl;
            break;
        }
    }
    
    ostringstream out;
    out << computer.get_accumulator();
    return out.str();
}
