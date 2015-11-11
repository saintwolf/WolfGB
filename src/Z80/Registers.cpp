#include "Registers.h"

Registers::Registers()
{
    Reset();
}

Registers::~Registers()
{

}

void Registers::Reset()
{
    af = 0;
    bc = 0;
    de = 0;
    hl = 0;
    pc = 0;
    sp = 0;
}

void Registers::SetFlag(Flags flag)
{
    f |= (int)flag;
}

void Registers::ClearFlag(Flags flag)
{
    f ^= (int)flag;
}

uint8_t Registers::GetFlag(Flags flag)
{
    return f & (int)flag;
}
