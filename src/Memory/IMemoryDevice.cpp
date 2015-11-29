#include "IMemoryDevice.h"

uint8_t IMemoryDevice::ReadByte(uint16_t address)
{
    return *GetMemoryPtr(address);
}

uint16_t IMemoryDevice::ReadWord(uint16_t address)
{
    return *GetMemoryPtr(address) + (*GetMemoryPtr(address + 1) << 8);
}

void IMemoryDevice::WriteByte(uint16_t address, uint8_t data)
{
    *GetMemoryPtr(address) = data;
}

void IMemoryDevice::WriteWord(uint16_t address, uint16_t data)
{
    *GetMemoryPtr(address) = data & 0x00FF;
    *GetMemoryPtr(address + 1) = data >> 8 & 0x00FF;
}
