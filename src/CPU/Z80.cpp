#include "Z80.h"

Z80::Z80()
{
    registers = new Z80Registers();
    instructions = new Z80Instructions(registers);
    Reset();
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

Z80Registers* Z80::GetRegisters()
{
    return registers;
}

