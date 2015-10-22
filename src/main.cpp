#include <stdio.h>

#include "Z80.h"

using namespace std;

void PrintRegisters();

Z80 z80;

int main()
{
    PrintRegisters();

    return 0;
}

void PrintRegisters()
{
    Z80Registers* r = z80.GetRegisters();

    printf("A: %d", r->a);
    printf(" | AF: %d\n", r->af);
    printf("F: %d\n", r->f);
    printf("B: %d", r->b);
    printf(" | BC: %d\n", r->bc);
    printf("C: %d\n", r->c);
    printf("D: %d", r->d);
    printf(" | DE: %d\n", r->de);
    printf("E: %d\n", r->e);
    printf("H: %d", r->h);
    printf(" | HL: %d\n", r->hl);
    printf("L: %d\n", r->l);
    printf("PC: %d\n", r->pc);
    printf("SP: %d\n", r->sp);
}
