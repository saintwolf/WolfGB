#ifndef Z80REGISTERS_H
#define Z80REGISTERS_H

#include <stdint.h>
#include "Z80Registers.h"

class Z80Registers
{
public:
    Z80Registers();
    virtual ~Z80Registers();

    union
    {
        struct
        {
            uint8_t f;
            uint8_t a;
        };
        uint16_t af;
    };
    union
    {
        struct
        {
            uint8_t c;
            uint8_t b;
        };
        uint16_t bc;
    };
    union
    {
        struct
        {
            uint8_t e;
            uint8_t d;
        };
        uint16_t de;
    };
    union
    {
        struct
        {
            uint8_t l;
            uint8_t h;
        };
        uint16_t hl;
    };

    uint16_t pc;
    uint16_t sp;

protected:
private:

};

#endif // Z80REGISTERS_H
