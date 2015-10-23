#include "Flags.h"

Flags::Flags(uint8_t* pFlags)
{
    this->pFlags = pFlags;
}

Flags::~Flags()
{
    //dtor
}

uint8_t Flags::GetFlag(FlagMasks flag)
{
    return *pFlags & flag;
}

void Flags::SetFlag(FlagMasks flag)
{
    *pFlags |= flag;
}
