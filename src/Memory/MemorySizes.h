#ifndef MEMORYSIZES_H
#define MEMORYSIZES_H


enum MemorySizes
{
    ROM_CARTRIDGE_SIZE = 0x8000, /**< 32kB Cartridge. */
    EXTERNAL_RAM_SIZE = 0x2000, /**<  8kB External (Cartridge) RAM. */
    WORKING_RAM_SIZE = 0x2000, /**<  8kB Working RAM. */
    HIGH_RAM_SIZE = 0xFF, /**<  127b Zero Page RAM. */
    VIDEO_RAM_SIZE = 0x2000, /**< 8KB Video RAM */
    OAM_SIZE = 0xA0,
};

#endif // MEMORYSIZES_H
