#include <SDL.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include "Z80.h"
#include "GDDB.h"

using namespace std;

SDL_Renderer* GetRenderer();

Z80* z80;
GDDB* gddb;

int SCREEN_WIDTH = 160;
int SCREEN_HEIGHT = 144;

int main(int argc, char *argv[])
{
    cout << "==================" << endl;
    cout << "Welcome to WolfGB!" << endl;
    cout << "==================" << endl << endl;


    cout << "Initialising LCD" << endl;
    SDL_Renderer* renderer = GetRenderer();

    cout << "Initialising GB Hardware" << endl;
    z80 = new Z80();
    z80->GetGPU()->SetScreenRenderer(renderer);
    z80->Reset();

    cout << "Initialising GDDB" << endl;
    gddb = new GDDB(z80);

    bool running = true;

    while (running)
    {
        gddb->Step();
        z80->Step();
    }
    return 0;
}

SDL_Renderer* GetRenderer()
{
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

            return renderer;
        }
    }
}
