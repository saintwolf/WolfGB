#ifndef Z80REGISTERS_H
#define Z80REGISTERS_H

#include <stdint.h>

enum class Flags
{
    C = 0x10,
    H = 0x20,
    N = 0x40,
    Z = 0x80,
};

class Registers
{
public:
    Registers();
    virtual ~Registers();

    void Reset();

    void SetFlag(Flags flag);
    void ClearFlag(Flags flag);
    uint8_t GetFlag(Flags flag);

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
