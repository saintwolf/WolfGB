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
    delete mmu;
    delete gpu;
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

int Z80::Step()
{
    uint8_t opcode = mmu->ReadByte(registers->pc++);
    instructions->ExecuteInstruction(opcode);

    uint8_t cycles;
    if (opcode == 0xCB)
    {
        cycles = CBClockCycles[opcode];
    }
    else
    {
        cycles = ClockCycles[opcode];
    }

    clock.m += cycles;
    clock.t += cycles >> 2;

    gpu->Step(cycles);

    return cycles;
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
