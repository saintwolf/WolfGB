#ifndef Z80INSTRUCTIONS_H
#define Z80INSTRUCTIONS_H

#include "Registers.h"

#include <stdio.h>

class Instructions
{
    Registers* registers;

public:
    Instructions(Registers* registers);
    virtual ~Instructions();

    void ExecuteInstruction(uint8_t opCode);

    // Instructions will return the m clock value of the operation
    int NOP()
    {
        printf("NOP TIME!!1one!");
        return 4;
    }

    //***********************//
    //***** 8-Bit Loads *****//
    //***********************//

    // LD r,n: Put immediate value n into r (2m)
    int LDrn_a();
    int LDrn_b();
    int LDrn_c();
    int LDrn_d();
    int LDrn_e();
    int LDrn_h();
    int LDrn_l();

    // LD r1,r2: Loads value from r2 to r1 (1m)
    int LDrr_aa();
    int LDrr_ab();
    int LDrr_ac();
    int LDrr_ad();
    int LDrr_ae();
    int LDrr_ah();
    int LDrr_al();
    int LDrr_ba();
    int LDrr_bb();
    int LDrr_bc();
    int LDrr_bd();
    int LDrr_be();
    int LDrr_bh();
    int LDrr_bl();
    int LDrr_ca();
    int LDrr_cb();
    int LDrr_cc();
    int LDrr_cd();
    int LDrr_ce();
    int LDrr_ch();
    int LDrr_cl();
    int LDrr_da();
    int LDrr_db();
    int LDrr_dc();
    int LDrr_dd();
    int LDrr_de();
    int LDrr_dh();
    int LDrr_dl();
    int LDrr_ea();
    int LDrr_eb();
    int LDrr_ec();
    int LDrr_ed();
    int LDrr_ee();
    int LDrr_eh();
    int LDrr_el();
    int LDrr_ha();
    int LDrr_hb();
    int LDrr_hc();
    int LDrr_hd();
    int LDrr_he();
    int LDrr_hh();
    int LDrr_hl();
    int LDrr_la();
    int LDrr_lb();
    int LDrr_lc();
    int LDrr_ld();
    int LDrr_le();
    int LDrr_lh();
    int LDrr_ll();

    // LD r,(HL): Loads value from memory address in HL to reg r (2m)
    int LDrHLm_a();
    int LDrHLm_b();
    int LDrHLm_c();
    int LDrHLm_d();
    int LDrHLm_e();
    int LDrHLm_h();
    int LDrHLm_l();

    // LD a,(RR): Loads value from memory address in 16 bit register to a (2m)
    int LDrBCm_a();
    int LDrDEm_a();

    // LD (HL),r: Loads value in reg r to memory address at HL (2m)
    int LDHLmr_a();
    int LDHLmr_b();
    int LDHLmr_c();
    int LDHLmr_d();
    int LDHLmr_e();
    int LDHLmr_h();
    int LDHLmr_l();


    int LDHLmn(); // LD (HL),n: Loads immediate value n into address in HL (12m)
    int LDNNmr_a(); // LD (nn),A: Loads value from a into address from operand (16m)

    int LDDHLmr_a(); // LDD (HL),A: Load A into (HL), decrement (HL) (8m)
    int LDDrHLm_a(); // LDD A,(HL): Load (HL) into A, decrement (HL) (8m)

    int LDIHLmr_a(); // LDI (HL),A: Load A into (HL), increment (HL) (8m)
    int LDIrHLm_a(); // LDI A,(HL): Load (HL) into A, increment (HL) (8m)

    //IO Loads
    int LDHrn_a(); // LDH A,n: Loads value from memory FF00+n into a (12m))
    int LDHnr_a(); // LDH n,A: Loads A into memory address FF00+n (12m)

    int LDIOCnr_a(); // LD IO+C,A: Put value from A into address at FF00 + C (8m)
    int LDIOCrn_a(); // LD A,IO+C: Put value from address at FF00 + C into A (8m)

    //************************//
    //***** 16-Bit Loads *****//
    //************************//

    // LD RR,nn: Put 16-bit immediate value nn into RR (12m)
    int LDBCnn();
    int LDDEnn();
    int LDHLnn();
    int LDSPnn();

    int LDSPHL(); // LD SP,HL: Put HL into SP (8m)
    int LDHLSPn(); // LDHL SP,n: Put SP+n effective address into HL. n is signed byte (12m)
    int LDNNmSP(); // LD (nn),SP: Put stack pointer at address nn (20m)

    // PUSH RR: Push register pair RR onto stack. Decrement SP twice. (16m)
    int PUSHAF();
    int PUSHBC();
    int PUSHDE();
    int PUSHHL();

    // POP RR: Pop 2 bytes off stack into register RR. Increment SP twice. (12m)
    int POPAF();
    int POPBC();
    int POPDE();
    int POPHL();

    //*********************//
    //***** 8-Bit ALU *****//
    //*********************//

    // ADD A,n: Add register n to register A (4m)
    int ADDAA();
    int ADDAB();
    int ADDAC();
    int ADDAD();
    int ADDAE();
    int ADDAH();
    int ADDAL();
    int ADDAHLm(); // Add memory in address HL to A (8m)
    int ADDAn(); // Read PC and add to A. Increment PC (8m)

    // ADC A,n: Add register n plus Carry flag to register A (4m)
    int ADCAA();
    int ADCAB();
    int ADCAC();
    int ADCAD();
    int ADCAE();
    int ADCAH();
    int ADCAL();
    int ADCAHLm(); // Add memory in address HL to A (8m)
    int ADCAn(); // Read PC and add to A. Increment PC (8m)

    // SUB A,n: Subtract register n from register A (4m)
    int SUBAA();
    int SUBAB();
    int SUBAC();
    int SUBAD();
    int SUBAE();
    int SUBAH();
    int SUBAL();
    int SUBAHLm(); // Subtract memory in address HL from A (8m)
    int SUBAn(); // Read PC and subtract from A. Increment PC (8m)

    // SBC A,n: Subtract register n + Carry flag from register A (4m)
    int SBCAA();
    int SBCAB();
    int SBCAC();
    int SBCAD();
    int SBCAE();
    int SBCAH();
    int SBCAL();
    int SBCAHLm(); // Subtract memory in address HL + carry from A (8m)
    int SBCAn(); // Read PC, add carry, and subtract from A. Increment PC (8m) @TODO: Doesn't exist on Gameboy?

    // AND n: Logically AND n with register A, result in A.
    int ANDAA();
    int ANDAB();
    int ANDAC();
    int ANDAD();
    int ANDAE();
    int ANDAH();
    int ANDAL();
    int ANDAHLm(); //(8m)
    int ANDAn(); // (8m)

    // OR n: Logical OR n with register A, result in A.
    int ORAA();
    int ORAB();
    int ORAC();
    int ORAD();
    int ORAE();
    int ORAH();
    int ORAL();
    int ORAHLm(); //(8m)
    int ORAn(); // (8m)

    // XOR n: Logical XOR n with register A, result in A.
    int XORAA();
    int XORAB();
    int XORAC();
    int XORAD();
    int XORAE();
    int XORAH();
    int XORAL();
    int XORAHLm(); //(8m)
    int XORAn(); // (8m)

    // CP n: Compare A with n. A-n but results are not stored
    int CPAA();
    int CPAB();
    int CPAC();
    int CPAD();
    int CPAE();
    int CPAH();
    int CPAL();
    int CPAHLm(); //(8m)
    int CPAn(); // (8m)

    // INC n: Increment register n
    int INCAA();
    int INCAB();
    int INCAC();
    int INCAD();
    int INCAE();
    int INCAH();
    int INCAL();
    int INCAHLm(); //(12m)

    // DEC n: Decrement register n
    int DECAA();
    int DECAB();
    int DECAC();
    int DECAD();
    int DECAE();
    int DECAH();
    int DECAL();
    int DECAHLm(); //(12m)

    //**********************//
    //***** 16-Bit ALU *****//
    //**********************//

    // ADD HL,RR: Add RR to HL (8m)
    int ADDHLBC();
    int ADDHLDE();
    int ADDHLHL();
    int ADDHLSP();

    int ADDSPn(); // ADD SP,n: Add n (signed immediate value) to SP (16m)

    // INC RR: Increment register RR (8m)
    int INCBC();
    int INCDE();
    int INCHL();
    int INCSP();

    // DEC RR: Decrement register RR (8m)
    int DECBC();
    int DECDE();
    int DECHL();
    int DECSP();

    //****************//
    //***** MISC *****//
    //****************//

    int DAA(); // Decimal adjust register A (4m)
    int CPL(); // Complement A register (Flip bits) (4m)
    int CCF(); // Complement carry flag (4m)
    int SCF(); // Set carry flag (4m)
    int HALT(); // Power down CPU until an interrupt occurs (4m)
    int STOP(); // Halt CPU and LCD display until button pressed (4m)

    int DI(); // Disables interrupts after the next instruction is executed (4m)
    int EI(); // Enables interrupts after the next instruction is executed (4m)

    //******************************//
    //***** Rotates and Shifts *****//
    //******************************//

    int RLCA(); // Rotate A left. Old bit 7 to carry flag
    int RLA(); // Rotate A left through Carry flag

    int RRCA(); // Rotate A right. Old bit 0 to carry flag
    int RRA(); // Rotate A right through Carry flag

    // CB RLC n: Rotate n left. Old bit 7 to carry flag (8m)
    int CBRLCA();
    int CBRLCB();
    int CBRLCC();
    int CBRLCD();
    int CBRLCE();
    int CBRLCH();
    int CBRLCL();
    int CBRLCHLm(); // Rotate memory at address HL left (16m)

    // CB RL n: Rotate n left through carry flag.(8m)
    int CBRLA();
    int CBRLB();
    int CBRLC();
    int CBRLD();
    int CBRLE();
    int CBRLH();
    int CBRLL();
    int CBRLHLm(); // Rotate memory at address HL left (16m)

    // CB RRC n: Rotate n right. Old bit 0 to carry flag (8m)
    int CBRRCA();
    int CBRRCB();
    int CBRRCC();
    int CBRRCD();
    int CBRRCE();
    int CBRRCH();
    int CBRRCL();
    int CBRRCHLm(); // Rotate memory at address HL left (16m)

    // CB RR n: Rotate n right through carry flag. (8m)
    int CBRRA();
    int CBRRB();
    int CBRRC();
    int CBRRD();
    int CBRRE();
    int CBRRH();
    int CBRRL();
    int CBRRHLm(); // Rotate memory at address HL left (16m)

    // CB SLA n: Shift n left into carry. LSB of n set to 0. (8m)
    int CBSLAA();
    int CBSLAB();
    int CBSLAC();
    int CBSLAD();
    int CBSLAE();
    int CBSLAH();
    int CBSLAL();
    int CBSLAHLm(); // Rotate memory at address HL left (16m)

    // CB SRA n: Shift n right into carry. MSB does not change. (8m)
    int CBSRAA();
    int CBSRAB();
    int CBSRAC();
    int CBSRAD();
    int CBSRAE();
    int CBSRAH();
    int CBSRAL();
    int CBSRAHLm(); // Rotate memory at address HL right (16m)

    // CB SRL n: Shift n right into carry. MSB set to 0. (8m)
    int CBSRLA();
    int CBSRLB();
    int CBSRLC();
    int CBSRLD();
    int CBSRLE();
    int CBSRLH();
    int CBSRLL();
    int CBSRLHLm(); // Rotate memory at address HL right (16m)

    //****************//
    //***** Jumps *****//
    //****************//

    int JPnn(); // Jump to 2 byte immediate value address (12m)
    int JPNZ(); // Jump if Z flag is not set
    int JPZ(); // Jump if Z flag is set
    int JPNC(); // Jump if C flag is not set
    int JPC(); // Jump if C flag is set
    int JPHLm(); // Jump to address in HL (4m)

    int JRn(); // Add n (immediate) to current address and jump
    int JRNZn(); // (8m)
    int JRZn(); // (8m)
    int JRNCn(); // (8m)
    int JRCn(); // (8m)

    //*****************//
    //***** Calls *****//
    //*****************//

    int CALLnn(); // Push address of next instruction onto stack and then jump to nn (12m)
    int CALLNZ();
    int CALLZ();
    int CALLNC();
    int CALLC();

    //********************//
    //***** Restarts *****//
    //********************//

    // RST n: Push present address onto stack. Jump to address (32m)
    int RST00();
    int RST08();
    int RST10();
    int RST18();
    int RST20();
    int RST28();
    int RST30();
    int RST38();

    //*******************//
    //***** Returns *****//
    //*******************//

    int RET(); // Pop 2 bytes from stack and jump to that address (8m)
    int RETNZ();
    int RETZ();
    int RETNC();
    int RETC();
    int RETI(); // Pop 2 bytes from stack, jump to that address and enable interrupts

    //****************************//
    //***** Bit Opcodes (CB) *****//
    //****************************//

    // BIT b,r: Test bit b in register r (8m)
    int CBBITbA();
    int CBBITbB();
    int CBBITbC();
    int CBBITbD();
    int CBBITbE();
    int CBBITbH();
    int CBBITbL();
    int CBBITbHLm(); // (16m)

    // SET b,r: Set bit b in register r (8m)
    int CBSETbA();
    int CBSETbB();
    int CBSETbC();
    int CBSETbD();
    int CBSETbE();
    int CBSETbH();
    int CBSETbL();
    int CBSETbHLm(); // (16m)

    // RES b,r: Reset bit b in register r (8m)
    int CBRESbA();
    int CBRESbB();
    int CBRESbC();
    int CBRESbD();
    int CBRESbE();
    int CBRESbH();
    int CBRESbL();
    int CBRESbHLm(); // (16m)

private:
    int LDrn(uint8_t* reg);
    int LDrr(uint8_t* destReg, uint8_t* sourceReg);
    int LDnRRm(uint8_t destReg, uint16_t* souceReg);
    int LDRRmn(uint16_t* destReg, uint8_t* souceReg);
    int LDRRnn(uint16_t* reg, uint16_t pc);
    int PUSHRR(uint16_t* reg);
    int POPRR(uint16_t* reg);
    int ADDAr(uint8_t* reg);
    int ADCAr(uint8_t* reg);
    int SUBAr(uint8_t* reg);
    int SBCAr(uint8_t* reg);
    int ANDAr(uint8_t* reg);
    int ORAr(uint8_t* reg);
    int XORAr(uint8_t* reg);
    int CPr(uint8_t* reg);
    int INCr(uint8_t* reg);
    int DECr(uint8_t* reg);
    int ADDHLRR(uint16_t* reg);
    int INCRR(uint16_t* reg);
    int DECRR(uint16_t* reg);

    int CBRLC(uint8_t* reg);
    int CBRL(uint8_t* reg);
    int CBRRC(uint8_t* reg);
    int CBRR(uint8_t* reg);
    int CBSLA(uint8_t* reg);
    int CBSRA(uint8_t* reg);
    int CBSRL(uint8_t* reg);
    int CBBITbr(uint8_t b, uint8_t* reg);
    int CBSETbr(uint8_t b, uint8_t* reg);
    int CBRESbr(uint8_t b, uint8_t* reg);

protected:
private:
    typedef int (Instructions::*FuncPtr)();
    // Array of function pointers
    FuncPtr OpCodes[0xFF] = {
        &Instructions::NOP, // 0x00
    };
};

#endif // Z80INSTRUCTIONS_H
