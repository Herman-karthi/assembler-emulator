/*****************************************************************************
TITLE: Claims																																
AUTHOR: R.murali karthikeya
ROLL NO.: 2301CS32
Declaration of Authorship
*****************************************************************************/
#include <bits/stdc++.h>
using namespace std;
int stackLimit = 1 << 23;
int PC = 0, SP = 0, regA = 0, regB = 0;
int instruction_cnt = 0;
vector<int> objectFile;
int orig_mem[1 << 24];
vector<string> mnemonics{"ldc", "adc", "ldl", "stl", "ldnl", "stnl", "add", "sub", "shl", "shr", "adj", "a2sp", "sp2a", "call", "return", "brz", "brlz", "br", "HALT"};
void handleOpcode(int opcode, int operand)
{
    int tempReg; // Temporary register to hold intermediate values

    // Handle different opcodes using if-else instead of switch-case
    if (opcode == 0) {
        // Move operand to regA and move old regA value to regB
        tempReg = regA;       // Save current value of regA in tempReg
        regA = operand;       // Move operand to regA
        regB = tempReg;       // Move old regA value to regB
    }
    else if (opcode == 1) {
        // Add operand to regA
        regA += operand;      // Increment regA by operand
    }
    else if (opcode == 2) {
        // Load value from memory at (SP + operand) into regA
        regB = regA;                 // Save regA into regB (backup)
        regA = orig_mem[SP + operand]; // Load value from memory at address (SP + operand)
    }
    else if (opcode == 3) {
        // Store value of regA into memory at (SP + operand)
        orig_mem[SP + operand] = regA; // Store regA at address (SP + operand)
        regA = regB;                  // Restore regA from regB
    }
    else if (opcode == 4) {
        // Load value from memory at address (regA + operand) into regA
        regA = orig_mem[regA + operand]; // Load from memory at address (regA + operand)
    }
    else if (opcode == 5) {
        // Store value of regB into memory at (regA + operand)
        orig_mem[regA + operand] = regB; // Store regB at (regA + operand)
    }
    else if (opcode == 6) {
        // Add regB to regA and store result in regA
        regA = regB + regA; // Sum regB and regA, store in regA
    }
    else if (opcode == 7) {
        // Subtract regA from regB and store result in regA
        regA = regB - regA; // Subtract regA from regB and store in regA
    }
    else if (opcode == 8) {
        // Perform left shift on regB by the value in regA
        regA = regB << regA; // Left shift regB by value of regA, store result in regA
    }
    else if (opcode == 9) {
        // Perform right shift on regB by the value in regA
        regA = regB >> regA; // Right shift regB by value of regA, store result in regA
    }
    else if (opcode == 10) {
        // Adjust stack pointer by operand value
        SP += operand;  // Increment the stack pointer by operand
    }
    else if (opcode == 11) {
        // Set stack pointer to regA and move regB to regA
        SP = regA;     // Set stack pointer to the value in regA
        regA = regB;   // Move regB to regA
    }
    else if (opcode == 12) {
        // Move stack pointer to regA and save regA to regB
        regB = regA;   // Copy regA to regB
        regA = SP;     // Set regA to the stack pointer value
    }
    else if (opcode == 13) {
        // Move program counter to regA and adjust PC to operand - 1
        regB = regA;   // Save regA to regB
        regA = PC;     // Move PC value to regA
        PC = operand - 1; // Set PC to operand - 1
    }
    else if (opcode == 14) {
        // Set program counter to regA and restore regA from regB
        PC = regA;     // Set PC to regA value
        regA = regB;   // Restore regA from regB
    }
    else if (opcode == 15) {
        // Conditional jump if regA is zero
        if (regA == 0) { // If regA is zero
            PC += operand; // Adjust program counter by operand
        }
    }
    else if (opcode == 16) {
        // Conditional jump if regA is negative
        if (regA < 0) { // If regA is negative
            PC += operand; // Adjust program counter by operand
        }
    }
    else if (opcode == 17) {
        // Unconditional jump by operand value
        PC += operand; // Adjust program counter by operand value
    }
    // else {
    //     // Invalid opcode
    //     cerr << "Error: Invalid opcode encountered: " << opcode << ". Aborting execution." << endl;
    //     exit(1); // Exit the program for invalid opcode
    // }
}

int processNextInstruction()
{
    // Check if the Program Counter (PC) is out of bounds
    if (PC >= static_cast<int>(objectFile.size()))
    {
       // cout << "Segmentation fault detected! Halting execution.";
       cout<<"completed\n ";
       return 0;
       // exit(0); // Terminate the program if out-of-bounds access occurs
    }

    // Extract the lower 8 bits (Opcode) from the current entry in the object file
    int currentOpcode = objectFile[PC] & 0xFF; // Last 8 bits for opcode

    // Extract the upper 24 bits (Operand) from the current entry in the object file
    int currentOperand = objectFile[PC] >> 8; // First 24 bits for operand

    // Output the corresponding mnemonic and operand in hexadecimal
    cout << mnemonics[currentOpcode];
    printf("\t%08X\n", currentOperand);

    // Special case: If the opcode is 18 (a specific instruction)
    if (currentOpcode == 18)
    {
        instruction_cnt++; // Increment instruction count
        return 0;          // Return early to signal end of function execution
    }

    // For other opcodes, execute the corresponding instruction
    handleOpcode(currentOpcode, currentOperand);
    instruction_cnt++; // Increment the global instruction count

    // Move the Program Counter to the next instruction
    PC++;

    // Check if the Stack Pointer exceeds the allowed stack limit
    if (SP > stackLimit)
    {
        cout << "Stack overflow detected! Terminating the program.";
        exit(0); // Exit if a stack overflow occurs
    }

    return 1; // Signal successful execution of the instruction
}

pair<long, bool> parseOperand(string &operand)
{
    if (operand.empty())
    {
        return {0, false}; // Return failure if the operand is empty
    }

    // Handle the operand as a string directly

    const int len = static_cast<int>(operand.size());

    vector<char> str(len + 1); // Create a vector for the string, ensuring space for the null terminator

    // Copy operand into the vector
  
    int i=0;
    while(i<len){
         str[i] = operand[i];
         ++i;
    }

    str[len] = '\0'; // Null-terminate the string

    char *endPtr;
    long int number;

    // Attempt to parse as decimal (base 10)
    
    number = strtol(str.data(), &endPtr, 10);
    if (*endPtr == '\0')
    {
        return {number, true}; // Successfully parsed as decimal
    }
    
    // Attempt to parse as decimal (base 8)

    number = strtol(str.data(), &endPtr, 8);
    if (*endPtr == '\0')
    {
        return {number, true}; // Successfully parsed as octal
    }


    // Attempt to parse as hexadecimal (base 16)
    number = strtol(str.data(), &endPtr, 16);
    if (*endPtr == '\0')
    {
        return {number, true}; // Successfully parsed as hexadecimal
    }

    // Return failure if both attempts fail
    return {-1, false};
}

void displayMemory()
{
     bool instructionProcessed = false;
    int startAddr = 0;
    string addressInput;
    cout << "Please provide the starting address (in hexadecimal): ";
    cin >> addressInput;

    // Convert the input address to a long integer
    pair<long, bool> parsedStartAddr = parseOperand(addressInput);
    startAddr = parsedStartAddr.first;

    cout << "Specify the number of memory words to display: ";
    string wordCountInput;
    cin >> wordCountInput;
     while (true)
        {
            instructionProcessed = processNextInstruction();
            // Exit loop when no more instructions can be processed
            if (!instructionProcessed)
            {
                break;
            }
        }
    // Convert the word count to an integer
    pair<long, bool> parsedWordCount = parseOperand(wordCountInput);
    int numWords = parsedWordCount.first;

    int currentAddr = startAddr;
    int endAddr = startAddr + numWords;

    // Loop through the memory starting from the specified address
    for (int addr = currentAddr; addr < endAddr; addr += 4)
    {
        printf("%08X: ", addr); // Output the current address

        // Output memory content in blocks of 4 words (each word is 4 bytes)
        for (int i = 0; i < 4 && addr + i < endAddr; ++i)
        {
            printf("%08X ", orig_mem[addr + i]); // Display each word in memory
        }

        printf("\n");
    }
}

int processCommand()
{
    // Initialize user input and instruction status
    string command;
    bool instructionProcessed = false;

    // Get command from the user
    cout << "Please enter a command for the emulator: ";
    cin >> command;

    // Check the entered command using switch-case for clarity and flexibility
    if (command == "step" || command == "STEP")
    {
        // Single step execution
        instructionProcessed = processNextInstruction();
        // Output reg states after execution
        printf("A = %08X, B = %08X, PC = %08X, SP = %08X\n", regA, regB, PC, SP);

        if (!instructionProcessed)
        {
            return 0; // Exit if no instruction processed
        }
    }
    else if (command == "run" || command == "RUN")
    {
        // Execute all instructions in a loop until completion
        while (true)
        {
            instructionProcessed = processNextInstruction();
            // Print reg state after each instruction
            printf("A = %08X, B = %08X, PC = %08X, SP = %08X\n", regA, regB, PC, SP);
            // Exit loop when no more instructions can be processed
            if (!instructionProcessed)
            {
                break;
            }
        }
        return 0; // Exit after processing all instructions
    }
    else if (command == "dump" || command == "DUMP")
    {
        // Show memory dump
        displayMemory();
    }
    else
    {
        // Handle invalid input
        cout << "Error: Invalid emulator command entered!" << endl;
        return 1; // Return failure code for invalid command
    }

    return 1; // Continue emulator operation unless explicitly stopped
}

int main(int argc, char *argv[])
{
    int dataValue;
    ifstream inputStream(argv[1], ios::binary); // Open the file in binary mode
    while (inputStream.read(reinterpret_cast<char *>(&dataValue), sizeof(int)))
    {
        objectFile.push_back(dataValue); // Read each 4-byte integer and push it into the vector
    }

    int idx = 0; // Initialize index for memory
    for (auto value : objectFile)
    {
        orig_mem[idx] = value; // Store the data into the memory array
        idx++;
    }
    // communicate();

    cout << "-step for single-step execution\n-dump for memory snapshot\n-run for full execution\nEnter the command: ";

    bool continueRunning = true;
    while (continueRunning)
    {
        if (processCommand())
        {             // Process the user input command
            continue; // If the command was executed successfully, continue the loop
        }
        continueRunning = false; // Exit the loop if the command was not valid
    }

    cout << "total instructions executed: " << instruction_cnt << endl;
    return 0;
}