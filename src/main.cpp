#include <SDL.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include "Z80.h"

using namespace std;

void PrintRegisters();
void PrintGddb();

Z80* z80;

int SCREEN_WIDTH = 160;
int SCREEN_HEIGHT = 144;

bool gddb = true;
uint16_t breakPoint = 0xFFFF;

int main(int argc, char *argv[])
{
    cout << "==================" << endl;
    cout << "Welcome to WolfGB!" << endl;
    cout << "==================" << endl << endl;


    cout << "Initializing LCD" << endl;
    SDL_Window* window = NULL;

    // Renders stuff to the window
    SDL_Renderer* renderer = NULL;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
    }
    else
    {
        //Create window
        window = SDL_CreateWindow( "WolfGB", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( window == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        }
        else
        {
            renderer = SDL_CreateRenderer(window, -1, 0);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            for (int x = 0; x < SCREEN_WIDTH; x++)
            {
                for (int y = 0; y < SCREEN_HEIGHT; y++)
                {
                    SDL_RenderDrawPoint(renderer, x, y);
                }
            }

            SDL_RenderPresent(renderer);

        }
    }



    cout << "Initializing GB Hardware" << endl;
    z80 = new Z80();
    z80->GetGPU()->SetScreenRenderer(renderer);
    z80->Reset();

    bool running = true;

    while (running)
    {
        if (gddb)
        {
            cout << endl;
            PrintRegisters();
            PrintGddb();
            cout << endl;
            printf("Executing instruction: 0x%X from 0x%X", z80->GetMMU()->GetMemoryRef(z80->GetRegisters()->pc), z80->GetRegisters()->pc);
            if (z80->GetMMU()->GetMemoryRef(z80->GetRegisters()->pc) == 0xCB)
            {
                printf("Executing CB instruction: 0x%X from 0x%X", z80->GetMMU()->GetMemoryRef(z80->GetRegisters()->pc + 1), z80->GetRegisters()->pc + 1);
            }
        }
        z80->Step();
    }
    return 0;
}

void PrintRegisters()
{
    Registers* r = z80->GetRegisters();

    printf("AF: 0x%X\n", r->af);
    printf("BC: 0x%X\n", r->bc);
    printf("DE: 0x%X\n", r->de);
    printf("HL: 0x%X\n", r->hl);
    printf("PC: 0x%X\n", r->pc);
    printf("SP: 0x%X\n", r->sp);
}

void PrintGddb()
{
    if (!gddb)
    {
        // cout << "GDDB is disabled. Type 'debug' to enable stepping, or press Enter to run the emulator." << endl;
        gddb = true; // @todo: remove this when instruction set is better
    }
    printf("PC: 0x%X | Next Instruction: 0x%X \n", z80->GetRegisters()->pc, z80->GetMMU()->ReadByte(z80->GetRegisters()->pc));
    cout << ">";
    string command;
    getline(cin, command);

    if (command.substr(0, 5) == "debug")
    {
        if (gddb)
            cout << "GDDB disabled!";
        else
            cout << "GDDB enabled!";
        cout << endl;
        gddb = !gddb;
    }

}
