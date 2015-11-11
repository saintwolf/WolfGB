#include <stdio.h>
#include <iostream>

#include "Z80.h"

using namespace std;

void PrintRegisters();

Z80* z80;

int main()
{
    cout << "Welcome to WolfGB!";
    cout << "Initializing GB Hardware" << endl;
    z80 = new Z80();
    z80->Reset();

    bool running = true;
    while (running)
    {
        PrintRegisters();

        cout << "Press Enter to step!";
        getchar();
        cout << endl;
        z80->Step();
    }
    return 0;
}

void PrintRegisters()
{
    Registers* r = z80->GetRegisters();

    printf("A: 0x%X", r->a);
    printf(" | AF: 0x%X\n", r->af);
    printf("F: 0x%X\n", r->f);
    printf("B: 0x%X", r->b);
    printf(" | BC: 0x%X\n", r->bc);
    printf("C: 0x%X\n", r->c);
    printf("D: 0x%X", r->d);
    printf(" | DE: 0x%X\n", r->de);
    printf("E: 0x%X\n", r->e);
    printf("H: 0x%X", r->h);
    printf(" | HL: 0x%X\n", r->hl);
    printf("L: 0x%X\n", r->l);
    printf("PC: 0x%X\n", r->pc);
    printf("SP: 0x%X\n", r->sp);
}
