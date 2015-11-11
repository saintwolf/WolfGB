#ifndef Z80_H
#define Z80_H

#include <stdint.h>
#include "Registers.h"
#include "Instructions.h"
#include "Memory/MMU.h"
#include "GPU/GPU.h"

class Z80
{

public:
    Z80();
    virtual ~Z80();

    void Reset();
    uint8_t Step(); // Steps through CPU. Returns the amount of M clock cycles

    Registers* GetRegisters();
    MMU* GetMMU();
    GPU* GetGPU();
protected:
private:

    struct Clock
    {
        uint8_t m;
        uint8_t t;
    };
    Clock clock;

    Registers* registers;
    Instructions* instructions;
    MMU* mmu;
    GPU* gpu;

    // Map of the number of m clock cycles by opcode
    uint8_t ClockCycles[0xFF] =
    {
        // 0x00
        // 0x10
        // 0x20
        // 0x30
        // 0x40
        // 0x50
        // 0x60
        // 0x70
        // 0x80
        // 0x90
        // 0xA0
        // 0xB0
        // 0xC0
        // 0xD0
        // 0xE0
        // 0xF0
    };
    uint8_t CBClockCycles[0xFF] =
    {
        // 0x00
        // 0x10
        // 0x20
        // 0x30
        // 0x40
        // 0x50
        // 0x60
        // 0x70
        // 0x80
        // 0x90
        // 0xA0
        // 0xB0
        // 0xC0
        // 0xD0
        // 0xE0
        // 0xF0
    };
};


#endif // Z80_H
