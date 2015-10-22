#ifndef Z80_H
#define Z80_H

#include <stdint.h>
#include "Z80Registers.h"
#include "Z80Instructions.h"

class Z80
{

public:
    Z80();
    virtual ~Z80();
    void Reset();
    Z80Registers* GetRegisters();
protected:
private:

    struct Clock
    {
        uint8_t m;
        uint8_t t;
    };

    Z80Registers* registers;
    Clock clock;

    Z80Instructions* instructions;

    typedef int (Z80Instructions::*FuncPtr)();
    // Array of function pointers
    FuncPtr OpCodes[0xFF] = {
        &Z80Instructions::NOP,
    };
};


#endif // Z80_H
