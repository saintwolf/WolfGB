#ifndef Z80FLAGS_H
#define Z80FLAGS_H

#include <stdint.h>

class Flags
{
public:
    Flags(uint8_t* pFlags);
    virtual ~Flags();

    enum FlagMasks
    {
        C = 0x10,
        H = 0x20,
        N = 0x40,
        Z = 0x80,
    };

    void SetFlag(FlagMasks flag);
    uint8_t GetFlag(FlagMasks flag);

protected:
private:
    uint8_t* pFlags;
};

#endif // Z80FLAGS_H
