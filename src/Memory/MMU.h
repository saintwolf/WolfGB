#ifndef MMU_H
#define MMU_H

#include<stdint.h>

class MMU
{
    public:
        MMU();
        virtual ~MMU();
    protected:
    private:
        uint8_t rom[0x8000];
        uint8_t wram[0x2000];
        uint8_t eram[0x2000];
        uint8_t zram[0xFF];
};

#endif // MMU_H
