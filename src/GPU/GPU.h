#ifndef GPU_H
#define GPU_H

#include <SDL.h>
#include <stdint.h>
#include "Z80/Registers.h"
#include "IMemoryDevice.h"

enum class ModeFlags
{
    HBlank = 0,
    VBlank = 1,
    OAMRead = 2, // Searching OAM RAM
    OAMWrite = 3, // Writing OAM data to display driver
};

class GPU: public IMemoryDevice
{
    public:
        GPU();
        virtual ~GPU();

        static const int ScreenWidth = 160;
        static const int ScreenHeight = 144;

        void Reset();
        void Step(uint8_t clockCycles);
        void SetRenderer(SDL_Renderer* renderer);

        uint8_t* GetMemoryPtr(uint16_t address);

    protected:
    private:
        ModeFlags lineMode;
        uint16_t modeClock;

        // GPU IO Registers
        uint8_t LCDC; // LCD Control (0xFF40)
        uint8_t STAT; // LCD Status (0xFF41)
        uint8_t ScrollY; // Y Scroll (0xFF42)
        uint8_t ScrollX; // X Scroll (0xFF43)
        uint8_t LY; // The current line being drawn/scanned
        uint8_t LYC; // LY Compare. Compares itself with LY. If the values are the same it causes STAT to set the coincidence flag.
        uint8_t DMA; // DMA Transfer and Start address
        uint8_t BGPalette; // Background palette data
        uint8_t ObjPalette0; // Object palette data 0
        uint8_t ObjPalette1; // Object palette data 1
        uint8_t WinPosY; // Window Y position
        uint8_t WinPosX; // Window X position

        uint8_t vram[MemorySizes.VIDEO_RAM_SIZE];
        uint8_t oam[MemorySizes.OAM_SIZE];

        // Pointer to the screen
        SDL_Renderer* windowRenderer;

        // Renders scanline
        void RenderScanLine();
        void RenderBgLine();
        void RenderWindowLine();
        void RenderOAMLine();

        void SetPixel(SDL_Renderer* renderer, uint8_t x, uint8_t y, uint8_t colour);

        // Functions to obtain information from LCDC 0xFF40
        bool LcdEnabled(); // bit 7
        uint16_t GetWindowTileMapAddress();
        bool WindowEnabled();
        uint16_t GetTileDataAddress(uint16_t tileMapAddress);
        uint16_t GetBgTileMapAddress(); // 9800-9BFF if off, 9C00-9FFF if on
        bool ObjectSize(); // False = 8x8, True = 8x16
        bool ObjectEnabled();
        bool BackgroundEnabled(); // bit 0

        uint16_t dummyVar = 0;
};

#endif // GPU_H
