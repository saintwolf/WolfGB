#ifndef Z80_H
#define Z80_H

#include <stdint.h>
#include "Registers.h"
#include "Instructions.h"

class Z80
{

public:
    Z80();
    virtual ~Z80();
    void Reset();
    void Step();
    Registers* GetRegisters();
protected:
private:

    struct Clock
    {
        uint8_t m;
        uint8_t t;
    };

    Registers* registers;
    Clock clock;

    Instructions* instructions;


};


#endif // Z80_H
