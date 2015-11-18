#ifndef GDDB_H
#define GDDB_H

#include "Z80.h"
#include <string>

using namespace std;

class GDDB
{
    public:
        GDDB(Z80*);
        virtual ~GDDB();

        bool enabled = true;

        void Step();
        void PrintRegisters();

    protected:
    private:
        void PrintNextInstr();
        void CommandPrompt();

        void HelpCommand();
        void BreakCommand(string bp);
        void DebugCommand();
        void ResetCommand();

        uint16_t BreakPoint = 0;

        Z80* z80;
};

#endif // GDDB_H
