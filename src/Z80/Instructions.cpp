#include "Instructions.h"

Instructions::Instructions(Registers* registers, MMU* mmu)
{
    this->registers = registers;
    this->mmu = mmu;
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
    *reg = mmu->ReadByte(++registers->pc);

}

int Instructions::LDrr(uint8_t *dest, uint8_t *source)
{
    *dest = *source;
    return 1;
}
