#include "Z80Registers.h"

Z80Registers::Z80Registers()
{
    af = 0;
    bc = 0;
    de = 0;
    hl = 0;
    pc = 0;
    sp = 0;
}

Z80Registers::~Z80Registers()
{
    //dtor
}
