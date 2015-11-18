#include "GDDB.h"

#include <iostream>



GDDB::GDDB(Z80* z80)
{
    this->z80 = z80;
}

GDDB::~GDDB()
{
    //dtor
}

void GDDB::Step()
{
    if (enabled)
    {
        if (BreakPoint == 0 || BreakPoint == z80->GetRegisters()->pc)
        {
            BreakPoint = 0;
            PrintNextInstr();
            CommandPrompt();
            printf("Executing instruction: 0x%X from 0x%X\n", z80->GetMMU()->GetMemoryRef(z80->GetRegisters()->pc), z80->GetRegisters()->pc);
            if (z80->GetMMU()->GetMemoryRef(z80->GetRegisters()->pc) == 0xCB)
            {
                printf("Executing CB instruction: 0x%X from 0x%X\n", z80->GetMMU()->GetMemoryRef(z80->GetRegisters()->pc + 1), z80->GetRegisters()->pc + 1);
            }
        }
    }
}

void GDDB::PrintRegisters()
{
    Registers* r = z80->GetRegisters();

    printf("AF: 0x%X\n", r->af);
    printf("BC: 0x%X\n", r->bc);
    printf("DE: 0x%X\n", r->de);
    printf("HL: 0x%X\n", r->hl);
    printf("PC: 0x%X\n", r->pc);
    printf("SP: 0x%X\n", r->sp);
}

void GDDB::PrintNextInstr()
{
    uint8_t opcode = z80->GetMMU()->ReadByte(z80->GetRegisters()->pc);
    printf("PC: 0x%X | Next OpCode: 0x%X \n", z80->GetRegisters()->pc, opcode);
    if (opcode == 0xCB)
    {
        printf("Next CB Opcode: 0x%X\n", z80->GetMMU()->ReadByte(z80->GetRegisters()->pc+1));
    }
}

void GDDB::CommandPrompt()
{
    bool loopCommand = true;
    // Loop the prompt until
    while (loopCommand)
    {

        cout << "gddb>";
        string command;
        getline(cin, command);

        // Process the command into an array
        string commandArray[10];

        int currPos = 0;
        int spacePos;
        int spaceCount = 0;
        int prevPos = 0;

        do
        {
            spacePos = command.find(" ", currPos);

            if (spacePos >= 0)
            {
                currPos = spacePos;
                commandArray[spaceCount] = command.substr(prevPos, currPos - prevPos);
                currPos++;
                prevPos = currPos;
                spaceCount++;
            }
        }
        while (spacePos >= 0 || spaceCount == 8);

        commandArray[spaceCount] = command.substr(prevPos,command.length());

        // Process commands through array
        if (commandArray[0] == "debug")
        {
            DebugCommand();
        }
        else if (commandArray[0] == "break")
        {
            BreakCommand(commandArray[1]);
        }
        else if (commandArray[0] == "show")
        {
            if (commandArray[1] == "regs")
            {
                PrintRegisters();
            }
            else
            {
                cout << "show Usage: regs" << endl;
            }
        }
        // Step to next command or breakpoint, nothing entered
        else if (command.length() == 0)
        {
            loopCommand = false;
        }
        else
        {
            HelpCommand();
        }
    }
}

void GDDB::HelpCommand()
{
    // Help menu
    cout << "--------" << endl;
    cout << "Commands" << endl;
    cout << "--------" << endl;
    cout << "debug\tEnables/Disables GDDB" << endl;
}

/** @brief Sets the breakpoint
 *
 * @param bp uint16_t
 * @return void
 *
 */
void GDDB::BreakCommand(string bp)
{
    try
    {
        BreakPoint = (uint16_t)stoi(bp, 0, 16);
        printf("Breaking at: 0x%X after next step!\n", BreakPoint);
    }
    catch (exception& e)
    {
        cout << "Invalid breakpoint value" << endl;
    }
}

void GDDB::DebugCommand()
{
    if (enabled)
        cout << "GDDB disabled!";
    else
        cout << "GDDB enabled!";
    cout << endl;
    enabled = !enabled;
}

void GDDB::ResetCommand()
{
    cout << "Resetting WolfGB" << endl;
    z80->Reset();
}
