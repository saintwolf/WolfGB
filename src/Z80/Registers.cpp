#include "Registers.h"

Registers::Registers()
{
    af = 0;
    bc = 0;
    de = 0;
    hl = 0;
    pc = 0;
    sp = 0;

    flags = new Flags(&f);
}

Registers::~Registers()
{
    delete flags;
}
