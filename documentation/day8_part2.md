# Day 8, Part 2 #

## Requirements ##

The input contains a list of instructions of the boot code. There are 3 operations, which each have one integer argument after them:
* `acc` which increments the *accumulator* by the value of the argument. The subsequent instruction is then run
* `jmp` which jumps to a different instruction to run next. The argument tells how many instructions up/down to jump. Note that `jmp +1` will move to the next instruction.
* `nop` which does nothing to the *accumulator* and results in the subsequent instruction run next.

The program in the input file needs one modification in order to run to completion, with the instruction pointer being one after the last line in the file. 

Either a jmp needs to flip to a nop or a nop needs to flip to a jmp. Fix the program so it runs to completion and return the final value in the accumulator


### Input Format ###

A series of instructions. Each instruction has two parts - the operation and the argument.
Examples: 
* `nop +0` 
* `acc +1` 
* `jmp +4`  

### Output Format ###

An integer - the value of the accumulator after the program runs to completion.

## Test Cases ##

### Given Test Cases ###

There is [one test case](../data/test_cases/day8_test1.txt) given. I don't anticipate needing other test cases.

### Custom Test Cases ###

None Needed

## Approach ##

Only outlining the main program changes here. See [Day 8 Part 1](day8_part1.md) for all the other details.

### New Method ###
The Program class will get a set_all_not_run method to clear the run flag on all the instructions.
The Program class will get a new get_program_line_count method to return the line count of the program.

### Main Program ###

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

## Things I learned ##

