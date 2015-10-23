#include "Z80.h"

Z80::Z80()
{
    registers = new Registers();
    instructions = new Instructions(registers);
    Reset();

    instructions->ExecuteInstruction(0); // @TODO: Just a NOP test
}

Z80::~Z80()
{
    delete instructions;
    delete registers;
}

void Z80::Reset()
{
    registers->af = 0;
    registers->bc = 0;
    registers->de = 0;
    registers->hl = 0;
    registers->pc = 0;
    registers->sp = 0;

    clock.m = 0;
    clock.t = 0;
}

void Z80::Step()
{
    // @TODO: Execute (PC)
    registers->pc++; // Increment PC after the instruction, to allow instructions to read the current operand.

}

Registers* Z80::GetRegisters()
{
    return registers;
}

