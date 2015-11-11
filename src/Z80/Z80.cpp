#include "Z80.h"

Z80::Z80()
{
    registers = new Registers();
    gpu = new GPU();
    mmu = new MMU(gpu);
    instructions = new Instructions(registers, mmu);
    Reset();
}

Z80::~Z80()
{
    delete instructions;
    delete gpu;
    delete mmu;
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

    registers->Reset();
    gpu->Reset();
    mmu->Reset();
}

uint8_t Z80::Step()
{
    instructions->ExecuteInstruction(mmu->ReadByte(registers->pc++));
    // @todo Increment clock
}

Registers* Z80::GetRegisters()
{
    return registers;
}

MMU* Z80::GetMMU()
{
    return mmu;
}

GPU* Z80::GetGPU()
{
    return gpu;
}
