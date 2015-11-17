#include "MMU.h"

#include <stdio.h>

MMU::MMU(GPU* gpu)
{
    this->gpu = gpu;
    Reset();
}

MMU::~MMU()
{
    //dtor
}

/** @brief Gets a pointer to a memory address to be accessed by Read/Write functions.
 *
 * @param address The memory address being accessed.
 * @return Pointer to the memory address.
 *
 */
uint8_t& MMU::GetMemoryRef(uint16_t address)
{
    switch (address & 0xF000)
    {
    // BIOS (256b)/ROM0
    case 0x0000:
        if (inBios)
        {
            if (address < 0x0100)
            {
                return bios[address];
            }
            else if (address == 0x0100)
            {
                inBios = false;
            }
        }
        return rom[address];

    // ROM 0 (32k)
    case 0x1000:
    case 0x2000:
    case 0x3000:
    case 0x4000:
    case 0x5000:
    case 0x6000:
    case 0x7000:
        return rom[address];

    // Video/Graphics RAM
    case 0x8000:
    case 0x9000:
        return gpu->GetMemoryRef(address);

    // External RAM
    case 0xA000:
    case 0xB000:
        return eram[address & 0x1FFF];

    // Working RAM
    case 0xC000:
    case 0xD000:
        return wram[address & 0x1FFF];

    // Working RAM shadow
    case 0xE000:
        return wram[address & 0x1FFF];

    case 0xF000:
        switch (address & 0x0F00)
        {
        case 0x000:
        case 0x100:
        case 0x200:
        case 0x300:
        case 0x400:
        case 0x500:
        case 0x600:
        case 0x700:
        case 0x800:
        case 0x900:
        case 0xA00:
        case 0xB00:
        case 0xC00:
        case 0xD00:
            return wram[address & 0x1FFF];

        // Sprite attribute memory (OAM)
        case 0xE00:


            gpu->GetMemoryRef(address);

        // High RAM / Memory Mapped IO
        case 0xF00:
            if (address < 0xFF80)
            {
                // Memory mapped IO
                switch (address & 0x00F0)
                {
                case 0x40:
                    gpu->GetMemoryRef(address);
                }
            }
            else
            {
                // Interrupt Enable
                if (address == 0xFFFF)
                {
                    // @todo Interrupt Enable?
                }
                // High RAM
                else
                {
                    return hram[address & 0x7F];
                }
            }

        default:
            printf("MEMORY ADDESS NOT FOUND: 0x%X", address);
            return randomint;
            break;
        }
    }
}


uint8_t MMU::ReadByte(uint16_t address)
{
    return GetMemoryRef(address);
}

uint16_t MMU::ReadWord(uint16_t address)
{
    return GetMemoryRef(address) + (GetMemoryRef(address + 1) << 8);
}

void MMU::WriteByte(uint16_t address, uint8_t data)
{
    GetMemoryRef(address) = data;
}

void MMU::WriteWord(uint16_t address, uint16_t data)
{
    GetMemoryRef(address) = data & 0x00FF;
    GetMemoryRef(address + 1) = data >> 8 & 0x00FF;
}

void MMU::Reset()
{
    // @todo Reset MMU
}
