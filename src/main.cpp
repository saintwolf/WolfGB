#include <SDL.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include "Z80/Z80.h"
#include "Debug/GDDB.h"

using namespace std;


SDL_Renderer* GetWindowRenderer();

SDL_Window* window = NULL;
SDL_Renderer* windowRenderer = NULL;

Z80* z80;
GDDB* gddb;

const int SCREEN_WIDTH = 160;
const int SCREEN_HEIGHT = 144;
const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

int main(int argc, char *argv[])
{

    cout << "==================" << endl;
    cout << "Welcome to WolfGB!" << endl;
    cout << "==================" << endl << endl;


    cout << "Initialising LCD" << endl;
    windowRenderer = GetWindowRenderer();

    cout << "Initialising GB Hardware" << endl;
    z80 = new Z80();
    z80->GetGPU()->SetRenderer(windowRenderer);
    cout << "Loading ROM" << endl;
    z80->GetMMU()->LoadRom("F:\\Users\\Saintwolf\\Documents\\Programming\\Gameboy\\cpu_instrs.gb");
    z80->Reset();

    cout << "Initialising GDDB" << endl;
    gddb = new GDDB(z80);

    bool running = true;

    const int CPUCLOCK_FRAME_TICKS = 70224;
    int cpuClock = 0;

    int startTime = SDL_GetTicks();

    while (running)
    {
        gddb->Step();
        cpuClock += z80->Step();

        if (cpuClock > CPUCLOCK_FRAME_TICKS)
        {
            SDL_Delay(1000.f / 60);
            cpuClock = 0;
            startTime = SDL_GetTicks();
        }
    }
    return 0;
}

SDL_Renderer* GetWindowRenderer()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
    }
    else
    {
        //Create window
        SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &windowRenderer);
        if( window == NULL )
    {
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        }
        else
        {
            return windowRenderer;
        }
    }
}
