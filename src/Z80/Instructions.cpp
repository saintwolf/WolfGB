#include "Instructions.h"

Instructions::Instructions(Registers* registers)
{
    this->registers = registers;
}

Instructions::~Instructions()
{
    //dtor
}

void Instructions::ExecuteInstruction(uint8_t instruction)
{
    int8_t opcode = instruction & 0xF;
    (this->*OpCodes[opcode])();
}

int Instructions::LDrn(uint8_t* reg)
{
    // @TODO: Need to implement MMU to read from memory
}

int Instructions::LDrr(uint8_t *dest, uint8_t *source)
{
    *dest = *source;
    return 1;
}
