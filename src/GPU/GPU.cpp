#include "GPU.h"
#include "Memory/IORegisters.h"

#include "stdio.h"

GPU::GPU()
{
    screenRenderer = NULL;
    Reset();
}

GPU::~GPU()
{
}

void GPU::Reset()
{
    // Clear screen
    for (int x = 0; x < ScreenWidth; x++)
    {
        for (int y = 0; y < ScreenHeight; y++)
        {
            SDL_RenderDrawPoint(screenRenderer, x, y);
        }
    }

    SDL_RenderPresent(screenRenderer);

    // Clear OAM
    for (int i = 0; i < OAM_SIZE; i++)
    {
        oam[i] = 0;
    }

    lineMode = ModeFlags::OAMRead;
    modeClock = 0;
    LCDC = 0;
    STAT = 0;
    ScrollY = 0;
    ScrollX = 0;
    LY = 0;
    LYC = 0;
    DMA = 0;
    BGPalette = 0;
    ObjPalette0 = 0;
    ObjPalette1 = 0;
    WinPosY = 0;
    WinPosX = 0;
}

void GPU::Step(uint8_t clockCycles)
{
    modeClock += clockCycles;
    switch (lineMode)
    {
    case ModeFlags::OAMRead:
        if (modeClock >= 20)
        {
            // Enter mode 3
            modeClock = 0;
            lineMode = ModeFlags::OAMWrite;
        }
        break;

    case ModeFlags::OAMWrite:
        if (modeClock >= 172)
        {
            // Enter HBlank, render scanline to display
            modeClock = 0;
            lineMode = ModeFlags::HBlank;

            RenderScanLine();
        }
        break;

    // HBlank
    // After the last HBlank, update the screen
    case ModeFlags::HBlank:
        if (modeClock >= 51)
        {
            // End of hblank for last scanline; render screen
            if (LY == 143)
            {
                // Enter VBlank
                lineMode = ModeFlags::VBlank;
                SDL_RenderPresent(screenRenderer);
            }
            // Go to OAM Read mode for next line
            else
            {
                lineMode = ModeFlags::OAMRead;
            }
            LY++;
            modeClock = 0;

        }

        break;

    case ModeFlags::VBlank:
        if (modeClock >= 114)
        {
            modeClock = 0;
            LY++;

            if (LY > 153)
            {
                // Restart scanning modes
                lineMode = ModeFlags::OAMRead;
                LY = 0;
            }
        }

        break;

    }
}

void GPU::SetScreenRenderer(SDL_Renderer* renderer)
{
    this->screenRenderer = renderer;
}

uint8_t& GPU::GetMemoryRef(uint16_t address)
{
    switch (address & 0xF000)
    {
    // Video/Graphics RAM
    case 0x8000:
    case 0x9000:
        return vram[address & 0x1FFF];

    case 0xF000:
        if ((address & 0xFF00) == 0xFE)
        {
            // @todo OAM
        }
        else if ((address & 0xFFF0) == 0xFF40)
        {
            // GPU IO Registers
            switch (address)
            {
            case IORegisters.LCDC: // 0xFF40
                return LCDC;
            case IORegisters.STAT:
                return STAT;
            case IORegisters.SCY:
                return ScrollY;
            case IORegisters.SCX:
                return ScrollX;
            case IORegisters.LY:
                return LY;
            case IORegisters.LYC:
                return LYC;
            case IORegisters.DMA:
                return DMA;
            case IORegisters.BGP:
                return BGPalette;
            case IORegisters.OBP0:
                return ObjPalette0;
            case IORegisters.OBP1:
                return ObjPalette1;
            case IORegisters.WY:
                return WinPosY;
            case IORegisters.WX:
                return WinPosX;
            }
        }
//    default:
//        printf("UNSUPPORTED GPU MEMORY LOCATION: 0x%X", address);
//        return 0;
    }
}

bool GPU::LcdEnabled()
{
    return LCDC >> 7 & 1;
}

uint16_t GPU::GetWindowTileMapAddress()
{
    if (LCDC >> 6 & 1) // Shift bit 6 to bit 0 and make sure it's the only value
        return 0x9C00;
    else
        return 0x9800;
}

bool GPU::WindowEnabled()
{
    return LCDC >> 5 & 1;
}

uint16_t GPU::GetTileDataAddress()
{
    if (LCDC >> 4 & 1)
        return 0x8000;
    else
        return 0x8800;
}

uint16_t GPU::GetBgTileMapAddress()
{
    if (LCDC >> 3 & 1)
        return 0x9C00;
    else
        return 0x9800;
}

bool GPU::ObjectEnabled()
{
    return LCDC >> 1 & 0;
}

bool GPU::BackgroundEnabled()
{
    return LCDC & 1;
}

void GPU::RenderScanLine()
{
    // Work out on which row we're rendering from the tile map
    uint8_t offsetY = LY + ScrollY;

    // Loop through each pixel on the line
    for (uint8_t currentX = 0; currentX < ScreenWidth; currentX++)
    {
        // Work out the pixel that we will be rendering
        uint8_t offsetX = ScrollX + currentX;

        uint8_t mapOffsetX = offsetX >> 3;
        uint8_t mapOffsetY = offsetY >> 3;

        uint8_t tileDataIndex = GetMemoryRef(GetBgTileMapAddress() + mapOffsetX * 32 + mapOffsetY);

        if (GetTileDataAddress() == 0x8800)
        {
            // We need to convert the signed integer into an unsigned integer
            // Let's shift it right 7 bits and clear all other bits
            if (tileDataIndex >> 7 & 1)
            {
                // If the sign bit it set, then add 128 to it
                tileDataIndex += 128;
            }
            else
            {
                // Otherwise, subtract 128 from it
                tileDataIndex -= 128;
            }
        }

        // X and Y coordinate within the tile
        uint8_t tileX = offsetX % 8;
        uint8_t tileY = offsetY % 8;

        // Points us to the correct Y line in the tile
        // There are 2 bytes per X line
        uint8_t dataAddress = tileDataIndex * 16 + tileY * 2;

        // If the X value is higher than 3, we need to point to the second byte
        if (tileX > 3)
        {
            dataAddress++;
        }

        // Grab the tile data from memory
        uint8_t tileData = GetMemoryRef(GetTileDataAddress() + tileDataIndex);

        // AND tileX with 3 to get the pixel number of the byte
        tileX &= 3;

        // Shift the tile value to get the pixel we need
        uint8_t pixel = tileData >> 2 * tileX;
        // Clear the rest of the byte
        pixel &= 0x3;

        LineBuffer[currentX] = pixel;

        for (int x = 0; x < ScreenWidth; x++)
        {
            uint8_t colour = LineBuffer[x];
            uint8_t colourFromPal = BGPalette >> colour * 2;
            switch (colourFromPal)
            {
            case 0:
                SDL_SetRenderDrawColor(screenRenderer, 255, 255, 255, 255);
                break;
            case 1:
                SDL_SetRenderDrawColor(screenRenderer, 192, 192, 192, 255);
                break;
            case 2:
                SDL_SetRenderDrawColor(screenRenderer, 64, 64, 64, 255);
                break;
            case 3:
                SDL_SetRenderDrawColor(screenRenderer, 0, 0, 0, 255);
                break;
            }
            SDL_RenderDrawPoint(screenRenderer, x, LY);
        }
    }
}
