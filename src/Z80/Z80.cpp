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
    clock.m = 0;
    clock.t = 0;

    registers->Reset();
    gpu->Reset();
    mmu->Reset();
}

uint8_t Z80::Step()
{
    uint8_t opcode = mmu->ReadByte(registers->pc++);
    instructions->ExecuteInstruction(opcode);
    gpu->Step(ClockCycles[opcode] >> 2);
    clock.m += ClockCycles[opcode];
    clock.t += ClockCycles[opcode] >> 2;
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
