#include "GPU.h"

#include "stdio.h"
#include <unistd.h>

GPU::GPU()
{
    windowRenderer = NULL;
    Reset();
}

GPU::~GPU()
{
}

void GPU::Reset()
{
    if (windowRenderer != NULL)
    {
        // Clear screen
        SDL_SetRenderDrawColor(windowRenderer, 0, 0, 0, 255);
        SDL_RenderClear(windowRenderer);

        SDL_RenderPresent(windowRenderer);
    }
    // Clear OAM
    for (int i = 0; i < MemorySizes.OAM_SIZE; i++)
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
    if (!LcdEnabled())
    {
        return;
    }

    modeClock += clockCycles;
    switch (lineMode)
    {
    case ModeFlags::OAMRead:
        if (modeClock >= 80)
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
        if (modeClock >= 204)
        {
            LY++;
            modeClock = 0;

            // End of hblank for last scanline; render screen
            if (LY == 144)
            {
                // Enter VBlank
                lineMode = ModeFlags::VBlank;
                SDL_RenderPresent(windowRenderer);
                usleep((1000 / 60) * 1000);
            }
            // Go to OAM Read mode for next line
            else
            {
                lineMode = ModeFlags::OAMRead;
            }
        }
        break;

    case ModeFlags::VBlank:
        if (modeClock >= 456)
        {
            modeClock = 0;
            LY++;

            if (LY == 154)
            {
                // Restart scanning modes
                lineMode = ModeFlags::OAMRead;
                LY = 0;
            }
        }

        break;

    }
}

void GPU::SetRenderer(SDL_Renderer* renderer)
{
    this->windowRenderer = renderer;
}

uint8_t* GPU::GetMemoryPtr(uint16_t address)
{
    switch (address & 0xF000)
    {
    // Video/Graphics RAM
    case 0x8000:
    case 0x9000:
        return &vram[address & 0x1FFF];

    case 0xF000:
        if ((address & 0xFF00) == 0xFE)
        {
            return &oam[address & 0x9F];
        }
        else if ((address & 0xFFF0) == 0xFF40)
        {
            // GPU IO Registers
            switch ((IORegisters)address)
            {
            case IORegisters::LCDC: // 0xFF40
                return &LCDC;
            case IORegisters::STAT:
                return &STAT;
            case IORegisters::SCY:
                return &ScrollY;
            case IORegisters::SCX:
                return &ScrollX;
            case IORegisters::LY:
                return &LY;
            case IORegisters::LYC:
                return &LYC;
            case IORegisters::DMA:
                return &DMA;
            case IORegisters::BGP:
                return &BGPalette;
            case IORegisters::OBP0:
                return &ObjPalette0;
            case IORegisters::OBP1:
                return &ObjPalette1;
            case IORegisters::WY:
                return &WinPosY;
            case IORegisters::WX:
                return &WinPosX;
            }
        }
//    default:
//        printf("UNSUPPORTED GPU MEMORY LOCATION: 0x%X", address);
//        return &0;
    }
}

void GPU::RenderScanLine()
{
    RenderBgLine();
    RenderWindowLine();
}

void GPU::RenderBgLine()
{
    uint16_t tileMapAddress = GetBgTileMapAddress();
    uint16_t tileDataAddress = GetTileDataAddress(tileMapAddress);

    uint8_t tileLow = ReadByte(tileDataAddress);
    uint8_t tileHigh = ReadByte(tileDataAddress + 1);
    int tileX = ScrollX >> 3;

    for (int x = 0; x < ScreenWidth; x++)
    {
        // Draw the current tile
        uint8_t paletteColour = tileLow >> (7 - tileX) & 0x1;
        paletteColour |= (tileHigh >> (7 - tileX) & 0x1) << 1;
        uint8_t colourFromPal = (BGPalette >> paletteColour * 2) & 0x3;

        SetPixel(windowRenderer, x, LY, colourFromPal);

        tileX++;
        if (tileX == 8)
        {
            // Finished with this tile, go to the next one
            tileX = 0;
            tileMapAddress++;
            tileDataAddress = GetTileDataAddress(tileMapAddress);
            tileLow = ReadByte(tileDataAddress);
            tileHigh = ReadByte(tileDataAddress + 1);
        }
    }

}

void GPU::RenderWindowLine()
{

}

void GPU::SetPixel(SDL_Renderer* renderer, uint8_t x, uint8_t y, uint8_t colour)
{
    switch (colour)
        {
        case 0:
            SDL_SetRenderDrawColor(windowRenderer, 255, 255, 255, 255);
            break;
        case 1:
            SDL_SetRenderDrawColor(windowRenderer, 170, 170, 170, 255);
            break;
        case 2:
            SDL_SetRenderDrawColor(windowRenderer, 85, 85, 85, 255);
            break;
        case 3:
            SDL_SetRenderDrawColor(windowRenderer, 0, 0, 0, 255);
            break;
        default:
            printf("GPU: Invalid colour from palette: %i\n", colour);
        }

        SDL_RenderDrawPoint(windowRenderer, x, y);
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

/** @brief Gets the tilemap address
 * Returns the tilemap address of the tile located at the start of the scanline
 * ((SCY + LY) * 32 + SCX)
 *
 * @return uint16_t
 *
 */
uint16_t GPU::GetBgTileMapAddress()
{
    uint16_t memoryAdd = (LCDC >> 3 & 1) == 0 ? 0x9800 : 0x9C00; // Get base address
    memoryAdd += ((ScrollY + LY & 0xFF) >> 3) * 32; // point to the correct line
    memoryAdd += ScrollX >> 3; // point to the start of the scanline
    return memoryAdd;
}

/** @brief Gets the tile data address
 * Gets the tile data address for the index given at tileMapAddress
 *
 * @param tileDataAddress uint8_t
 * @return uint16_t
 *
 */
uint16_t GPU::GetTileDataAddress(uint16_t tileMapAddress)
{
    uint8_t tileNum = ReadByte(tileMapAddress);
    uint16_t tileDataAddress = (LCDC >> 4 & 1) == 0 ? 0x9000 : 0x8000;

    if (tileDataAddress == 0x9000)
    {
        // Cast to signed integer when we use tilenum
        tileDataAddress += (int8_t)tileNum * 16;
    }
    else
    {
        tileDataAddress += tileNum * 16;
    }

    // Point to the correct row within the tile
    tileDataAddress += (ScrollY + LY & 0x7) * 2;
    return tileDataAddress;
}

bool GPU::ObjectEnabled()
{
    return LCDC >> 1 & 0;
}

bool GPU::BackgroundEnabled()
{
    return LCDC & 1;
}
