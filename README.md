these are the assembler and an emulator for a assembly language
=============================
Program format and structure:
=============================
1. The assembler:

    * with two data structures to store respectively labels and mnemonic table with expected operands.
    * uses a single routine for both passes.
    * detects label errors.
    * consistent and sensible formatting with sensible program structure. 
    * sensible variable, function & type names with explanatory comments.
		* advanced listing file.
		* assembles test programs.
	  * can implements and demonstrates the instruction SET.

2. The emulator :

    * with two data structures to store respectively memory, mnemonic table with expected operands
    * loads object file, 
    * by giving the instructions like "-befor/-after" , the program can produce memory dump before/after execution.
    * by giving the instruction like "-trace", the program prints the result of program executed.
    * detects errant programs.


=============================
Testing:
=============================

1. The assembler:

I have tested eight examples shown as bellow. The errors only occurred in the file test2.asm and the errors are logged and shown in terminal.

The others were tested without any errors detected and produced three output files, listing file, 
error log file and machine readable object file, for each example. 

After comparing the listing files and object file of all test files
with the implemented result given on the assignment, both of them show no difference 
from those provided.
 

1)
#Input: ./assembler test1.asm
#Output: 
a. test1.l 
b. test1.log (without any error messages only warning)
c. test1.o

2)
#Input:./assembler test2.asm
#Output: 
 
a. test2.log (with error messages shown as below but also in log file)


error messages:
ERROR: Duplicate Label at line 4
WARNING: Incorrect label format at line 10
ERROR: Unknown Symbol as operand at line 5
ERROR: Unknown Symbol as operand at line 6
ERROR: Operand expected at line 7
ERROR: Operand not expected at line 8
ERROR: Unknown Symbol as operand at line 9
ERROR: Mnemonic not defined at line 11
ERROR: Mnemonic not defined at line 12

3)
#Input:./assembler test3.asm
#Output: 
a. test3.l 
b. test3.log (without any error messages)
c. test3.o

4)
#Input:./assembler test4.asm
#Output: 
a. test4.l 
b. test4.log (without any error messages)
c. test4.o

5)
#Input:./assembler sampletest.asm
#Output: 

a. test5.log (with any error messages)
Invalid mnemonic at line: 3
Invalid operand format at line: 4
Missing operand at line: 6
goob label does not exist at line: 7
Invalid operand at line: 10
Warning: label "good" not used


7)
#Input:./assembler bubble.asm
#Output: 
a. bubble.l 
b. bubble.log (without any error messages)
c. bubble.o


2. The emulator:

I have tested eight examples shown as bellow. The errors only occurred in the file test1.o and 
the program immediately stopped with an error message of infinite loop on the screen. 

The others were tested without any errors detected and produced excution data both on the screen 


1)
#Input:./emulator test1.o


Infinite loop detected


2)
#Input:./emulator test3.o


no output for set instruction assembly code

3)
#Input:./emulator test4.o


47653 number of instructions executed


6)
#Input:./emulator bubble.o


1044 number of instructions executed
