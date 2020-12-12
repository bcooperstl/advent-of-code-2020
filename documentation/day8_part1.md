# Day 8, Part 1 #

## Requirements ##

The input contains a list of instructions of the boot code. There are 3 operations, which each have one integer argument after them:
* `acc` which increments the *accumulator* by the value of the argument. The subsequent instruction is then run
* `jmp` which jumps to a different instruction to run next. The argument tells how many instructions up/down to jump. Note that `jmp +1` will move to the next instruction.
* `nop` which does nothing to the *accumulator* and results in the subsequent instruction run next.

The program in the input file needs to be run until an instruction is repeated.

We need to find the value of the *accumulator* before this repeated insturction is run.

### Input Format ###

A series of instructions. Each instruction has two parts - the operation and the argument.
Examples: 
* `nop +0` 
* `acc +1` 
* `jmp +4`  

### Output Format ###

An integer - the value of the accumulator before the repeated instruction is run.

## Test Cases ##

### Given Test Cases ###

There is [one test case](../data/test_cases/day8_test1.txt) given. I don't anticipate needing other test cases.

### Custom Test Cases ###

None Needed

## Approach ##

### Classes to Create ###
1) Computer class
* holds the instruction_pointer and accumulator variables
* holds a reference to the Operations class singleton
* has methods to run an instruction
* has methods to get and set the instruction_pointer and accumulator values that will be called by the derivced operaitons

2) Instruction class
* holds the instruction from the program.
* will have a boolean flag to indicate if an instruction has been run. This will be used to determine when to end part 1

3) Program class
* holds a list of instructions in order.

4) Operation superclass and 3 derived classes
* Virtual method run_operation(Instruction *, Computer *) which does the work

5) Operations class
* singleton class
* contains instances of all the derived operations classes
* has a map to lookup an Operation object given the operation name

### Input Parsing: ###
* Use existing method to read in as a list of strings, split by the spaces
* For each line, create an instruction from the first as a string and second as an integer, and put these in a Program

### run_operation functionality ###

acc: increment the Computer's accumulator by the argument and the instruction pointer by 1.  
jmp: increment the Computer's instruction_pointer by the argument.  
nop: increment the Computer's insturction_pointer by 1.  

### Main Program ###

* Create a Computer, which will in turn grab the Operations singleton, which will result in the 3 derived Operations being created
* Create a Program from the input file
* Loop through the Program
    * Get the next instruction to Execute from the Computer's instruction pointer.
    * If that insturction has its run flag as true, we are done
        * Store the accumulator value from the Computer to be returned and discontinue the loop.
    * Call the Computer's run_instruction method, passing the Instruction. The Computer will fetch the Operation and call its run_operation method to do the processing.
    * Mark the instruction as ran
* Return the stored accumulator value.

## Things I learned ##

I thought about making the Operation superclass a friend of Computer, but quickly found out that friendship does not inherit, so that was out.

