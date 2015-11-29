#ifndef Z80INSTRUCTIONS_H
#define Z80INSTRUCTIONS_H

#include <stdio.h>

#include "Registers.h"
#include "MMU.h"

class Instructions
{
    Registers* registers;
    MMU* mmu;

public:
    Instructions(Registers* registers, MMU* mmu);
    virtual ~Instructions();

    /** @brief Executes instruction from function pointer based on opcode
     * Does not increment PC. This is done from the Z80::Step() method.
     *
     * @return void
     *
     */
    void ExecuteInstruction(uint8_t opCode);

private:

    /** @brief Loads 8-bit immediate value
     * Loads 8-bit immediate value from memory address pointed by PC.
     * Increments PC once.
     *
     * @return Value of memory pointed by PC
     *
     */
    uint8_t LoadImmediate8();

    /** @brief Loads 16-bit immediate value
     * Loads 16-bit immediate value from memory address pointed by PC.
     * Increments PC twice as value will fill 2 memory addresses.
     *
     * @return Value of memory pointed by PC
     *
     */
    uint16_t LoadImmediate16();

    /** @brief Decrements SP, then puts data into address at SP
     *
     * @param uint16_t data The data.
     *
     */
    void PushStack (uint16_t data);

    /** @brief Fetches data at SP, then increments SP.
     *
     * @return Data from address at SP
     *
     */
    uint16_t PopStack ();

    void CheckHalfCarry(uint8_t a, uint8_t b, bool carryIn = false);
    void CheckHalfCarry(uint16_t a, uint16_t b, bool carryIn = false);
    void CheckCarry(uint8_t a, uint8_t b, bool carryIn = false);
    void CheckCarry(uint16_t a, uint16_t b, bool carryIn = false);
    void CheckHalfBorrow(uint8_t a, uint8_t b, bool carryIn = false);
    void CheckBorrow(uint8_t a, uint8_t b, bool carryIn = false);

    void CheckZero(uint8_t result);

    // A group of functions to reduce duplicated code for some of the opcodes.

    void LDrn(uint8_t* reg);
    void LDrr(uint8_t* dest, uint8_t* source);
    void LDrRRm(uint8_t* dest, uint16_t* source);
    void LDRRmr(uint16_t* dest, uint8_t* source);
    void PUSHRR(uint16_t* reg);
    void POPRR(uint16_t* reg);
    void ADDAr(uint8_t* reg);
    void ADCAr(uint8_t* reg);
    void SUBAr(uint8_t* reg);
    void SBCAr(uint8_t* reg);
    void ANDAr(uint8_t* reg);
    void ORAr(uint8_t* reg);
    void XORAr(uint8_t* reg);
    void CPr(uint8_t* reg);
    void INCr(uint8_t* reg);
    void DECr(uint8_t* reg);
    void ADDHLRR(uint16_t* reg);
    void INCRR(uint16_t* reg);
    void DECRR(uint16_t* reg);

    void RSTN(uint8_t offset);

    void CBRLCr(uint8_t* reg);
    void CBRLr(uint8_t* reg);
    void CBRRCr(uint8_t* reg);
    void CBRRr(uint8_t* reg);
    void CBSLAr(uint8_t* reg);
    void CBSRAr(uint8_t* reg);
    void CBSRLr(uint8_t* reg);
    void CBBITbr(uint8_t b, uint8_t* reg);
    void CBSETbr(uint8_t b, uint8_t* reg);
    void CBRESbr(uint8_t b, uint8_t* reg);
    void CBSWAPn(uint8_t* reg);

    //***************************//
    //***START OF INSTRUCTIONS***//
    //***************************//
    int NOP();

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

    // LD r,(RR): Loads value from memory address in RR to reg r (2m)
    int LDrBCm_a();
    int LDrDEm_a();
    int LDrHLm_a();
    int LDrHLm_b();
    int LDrHLm_c();
    int LDrHLm_d();
    int LDrHLm_e();
    int LDrHLm_h();
    int LDrHLm_l();

    // LD (RR),r: Loads value in reg r to memory address at RR (2m)
    int LDBCmr_a(); // (8m)
    int LDDEmr_a();
    int LDHLmr_a();
    int LDHLmr_b();
    int LDHLmr_c();
    int LDHLmr_d();
    int LDHLmr_e();
    int LDHLmr_h();
    int LDHLmr_l();

    int LDHLmn(); // LD (HL),n: Loads immediate value n into address in HL (12m)
    int LDNNmr_a(); // LD (nn),A: Loads value from A into address from operand (16m)
    int LDrNNm_a();

    int LDDHLmr_a(); // LDD (HL),A: Load A into (HL), decrement (HL) (8m)
    int LDDrHLm_a(); // LDD A,(HL): Load (HL) into A, decrement (HL) (8m)

    int LDIHLmr_a(); // LDI (HL),A: Load A into (HL), increment (HL) (8m)
    int LDIrHLm_a(); // LDI A,(HL): Load (HL) into A, increment (HL) (8m)

    //IO Loads
    int LDHrn_a(); // LDH A,n: Loads value from memory FF00+n into a (12m))
    int LDHnr_a(); // LDH n,A: Loads A into memory address FF00+n (12m)

    int LDIOCrn_a(); // LD A,IO+C: Put value from address at FF00 + C into A (8m)
    int LDIOCnr_a(); // LD IO+C,A: Put value from A into address at FF00 + C (8m)

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
    int SBCAn(); // Read PC, add carry, and subtract from A. Increment PC (8m) @todo Doesn't exist on Gameboy?

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
    int CPr_a();
    int CPr_b();
    int CPr_c();
    int CPr_d();
    int CPr_e();
    int CPr_h();
    int CPr_l();
    int CPHLm(); //(8m)
    int CPn(); // (8m)

    // INC n: Increment register n
    int INCA();
    int INCB();
    int INCC();
    int INCD();
    int INCE();
    int INCH();
    int INCL();
    int INCHLm(); //(12m)

    // DEC n: Decrement register n
    int DECA();
    int DECB();
    int DECC();
    int DECD();
    int DECE();
    int DECH();
    int DECL();
    int DECHLm(); //(12m)

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
    int JPNZnn(); // Jump if Z flag is not set
    int JPZnn(); // Jump if Z flag is set
    int JPNCnn(); // Jump if C flag is not set
    int JPCnn(); // Jump if C flag is set
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
    int CALLNZnn();
    int CALLZnn();
    int CALLNCnn();
    int CALLCnn();

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
    int CBBIT0A();
    int CBBIT1A();
    int CBBIT2A();
    int CBBIT3A();
    int CBBIT4A();
    int CBBIT5A();
    int CBBIT6A();
    int CBBIT7A();
    int CBBIT0B();
    int CBBIT1B();
    int CBBIT2B();
    int CBBIT3B();
    int CBBIT4B();
    int CBBIT5B();
    int CBBIT6B();
    int CBBIT7B();
    int CBBIT0C();
    int CBBIT1C();
    int CBBIT2C();
    int CBBIT3C();
    int CBBIT4C();
    int CBBIT5C();
    int CBBIT6C();
    int CBBIT7C();
    int CBBIT0D();
    int CBBIT1D();
    int CBBIT2D();
    int CBBIT3D();
    int CBBIT4D();
    int CBBIT5D();
    int CBBIT6D();
    int CBBIT7D();
    int CBBIT0E();
    int CBBIT1E();
    int CBBIT2E();
    int CBBIT3E();
    int CBBIT4E();
    int CBBIT5E();
    int CBBIT6E();
    int CBBIT7E();
    int CBBIT0H();
    int CBBIT1H();
    int CBBIT2H();
    int CBBIT3H();
    int CBBIT4H();
    int CBBIT5H();
    int CBBIT6H();
    int CBBIT7H();
    int CBBIT0L();
    int CBBIT1L();
    int CBBIT2L();
    int CBBIT3L();
    int CBBIT4L();
    int CBBIT5L();
    int CBBIT6L();
    int CBBIT7L();
    int CBBIT0HLm(); // (16m)
    int CBBIT1HLm();
    int CBBIT2HLm();
    int CBBIT3HLm();
    int CBBIT4HLm();
    int CBBIT5HLm();
    int CBBIT6HLm();
    int CBBIT7HLm();

    // SET b,r: Set bit b in register r (8m)
    int CBSET0A();
    int CBSET1A();
    int CBSET2A();
    int CBSET3A();
    int CBSET4A();
    int CBSET5A();
    int CBSET6A();
    int CBSET7A();
    int CBSET0B();
    int CBSET1B();
    int CBSET2B();
    int CBSET3B();
    int CBSET4B();
    int CBSET5B();
    int CBSET6B();
    int CBSET7B();
    int CBSET0C();
    int CBSET1C();
    int CBSET2C();
    int CBSET3C();
    int CBSET4C();
    int CBSET5C();
    int CBSET6C();
    int CBSET7C();
    int CBSET0D();
    int CBSET1D();
    int CBSET2D();
    int CBSET3D();
    int CBSET4D();
    int CBSET5D();
    int CBSET6D();
    int CBSET7D();
    int CBSET0E();
    int CBSET1E();
    int CBSET2E();
    int CBSET3E();
    int CBSET4E();
    int CBSET5E();
    int CBSET6E();
    int CBSET7E();
    int CBSET0H();
    int CBSET1H();
    int CBSET2H();
    int CBSET3H();
    int CBSET4H();
    int CBSET5H();
    int CBSET6H();
    int CBSET7H();
    int CBSET0L();
    int CBSET1L();
    int CBSET2L();
    int CBSET3L();
    int CBSET4L();
    int CBSET5L();
    int CBSET6L();
    int CBSET7L();
    int CBSET0HLm(); // (16m)
    int CBSET1HLm();
    int CBSET2HLm();
    int CBSET3HLm();
    int CBSET4HLm();
    int CBSET5HLm();
    int CBSET6HLm();
    int CBSET7HLm();

    // RES b,r: Reset bit b in register r (8m)
    int CBRES0A();
    int CBRES1A();
    int CBRES2A();
    int CBRES3A();
    int CBRES4A();
    int CBRES5A();
    int CBRES6A();
    int CBRES7A();
    int CBRES0B();
    int CBRES1B();
    int CBRES2B();
    int CBRES3B();
    int CBRES4B();
    int CBRES5B();
    int CBRES6B();
    int CBRES7B();
    int CBRES0C();
    int CBRES1C();
    int CBRES2C();
    int CBRES3C();
    int CBRES4C();
    int CBRES5C();
    int CBRES6C();
    int CBRES7C();
    int CBRES0D();
    int CBRES1D();
    int CBRES2D();
    int CBRES3D();
    int CBRES4D();
    int CBRES5D();
    int CBRES6D();
    int CBRES7D();
    int CBRES0E();
    int CBRES1E();
    int CBRES2E();
    int CBRES3E();
    int CBRES4E();
    int CBRES5E();
    int CBRES6E();
    int CBRES7E();
    int CBRES0H();
    int CBRES1H();
    int CBRES2H();
    int CBRES3H();
    int CBRES4H();
    int CBRES5H();
    int CBRES6H();
    int CBRES7H();
    int CBRES0L();
    int CBRES1L();
    int CBRES2L();
    int CBRES3L();
    int CBRES4L();
    int CBRES5L();
    int CBRES6L();
    int CBRES7L();
    int CBRES0HLm(); // (16m)
    int CBRES1HLm();
    int CBRES2HLm();
    int CBRES3HLm();
    int CBRES4HLm();
    int CBRES5HLm();
    int CBRES6HLm();
    int CBRES7HLm();

    // SWAP n: Swap upper and lower nibbles of n (8m)
    int CBSWAPA();
    int CBSWAPB();
    int CBSWAPC();
    int CBSWAPD();
    int CBSWAPE();
    int CBSWAPH();
    int CBSWAPL();
    int CBSWAPHLm();

    int CBInst();

    // Type definition for arrays of function pointers (Used for regular and CB instruction set)
    typedef int (Instructions::*FuncPtr)();
    // Array of function pointers
    FuncPtr OpCodes[256] =
    {
        // 0x00
        &Instructions::NOP,
        &Instructions::LDBCnn,
        &Instructions::LDBCmr_a,
        &Instructions::INCBC,
        &Instructions::INCB,
        &Instructions::DECB,
        &Instructions::LDrn_b,
        &Instructions::RLCA,
        &Instructions::LDNNmSP,
        &Instructions::ADDHLBC,
        &Instructions::LDrBCm_a,
        &Instructions::DECBC,
        &Instructions::INCC,
        &Instructions::DECC,
        &Instructions::LDrn_c,
        &Instructions::RRCA,

        // 0x10
        &Instructions::STOP,
        &Instructions::LDDEnn,
        &Instructions::LDDEmr_a,
        &Instructions::INCDE,
        &Instructions::INCD,
        &Instructions::DECD,
        &Instructions::LDrn_d,
        &Instructions::RLA,
        &Instructions::JRn,
        &Instructions::ADDHLDE,
        &Instructions::LDrDEm_a,
        &Instructions::DECDE,
        &Instructions::INCE,
        &Instructions::DECE,
        &Instructions::LDrn_e,
        &Instructions::RRA,

        // 0x20
        &Instructions::JRNZn,
        &Instructions::LDHLnn,
        &Instructions::LDIHLmr_a,
        &Instructions::INCHL,
        &Instructions::INCH,
        &Instructions::DECH,
        &Instructions::LDrn_h,
        &Instructions::DAA,
        &Instructions::JRZn,
        &Instructions::ADDHLHL,
        &Instructions::LDIrHLm_a,
        &Instructions::DECHL,
        &Instructions::INCL,
        &Instructions::DECL,
        &Instructions::LDrn_l,
        &Instructions::CPL,

        // 0x30
        &Instructions::JRNCn,
        &Instructions::LDSPnn,
        &Instructions::LDDHLmr_a,
        &Instructions::INCSP,
        &Instructions::INCHLm,
        &Instructions::DECHLm,
        &Instructions::LDHLmn,
        &Instructions::SCF,
        &Instructions::JRCn,
        &Instructions::ADDHLSP,
        &Instructions::LDDrHLm_a,
        &Instructions::DECSP,
        &Instructions::INCA,
        &Instructions::DECA,
        &Instructions::LDrn_a,
        &Instructions::CCF,

        // 0x40
        &Instructions::LDrr_bb,
        &Instructions::LDrr_bc,
        &Instructions::LDrr_bd,
        &Instructions::LDrr_be,
        &Instructions::LDrr_bh,
        &Instructions::LDrr_bl,
        &Instructions::LDrHLm_b,
        &Instructions::LDrr_ba,
        &Instructions::LDrr_cb,
        &Instructions::LDrr_cc,
        &Instructions::LDrr_cd,
        &Instructions::LDrr_ce,
        &Instructions::LDrr_ch,
        &Instructions::LDrr_cl,
        &Instructions::LDrHLm_c,
        &Instructions::LDrr_ca,

        // 0x50
        &Instructions::LDrr_db,
        &Instructions::LDrr_dc,
        &Instructions::LDrr_dd,
        &Instructions::LDrr_de,
        &Instructions::LDrr_dh,
        &Instructions::LDrr_dl,
        &Instructions::LDrHLm_d,
        &Instructions::LDrr_da,
        &Instructions::LDrr_eb,
        &Instructions::LDrr_ec,
        &Instructions::LDrr_ed,
        &Instructions::LDrr_ee,
        &Instructions::LDrr_eh,
        &Instructions::LDrr_el,
        &Instructions::LDrHLm_e,
        &Instructions::LDrr_ea,

        // 0x60
        &Instructions::LDrr_hb,
        &Instructions::LDrr_hc,
        &Instructions::LDrr_hd,
        &Instructions::LDrr_he,
        &Instructions::LDrr_hh,
        &Instructions::LDrr_hl,
        &Instructions::LDrHLm_h,
        &Instructions::LDrr_ha,
        &Instructions::LDrr_lb,
        &Instructions::LDrr_lc,
        &Instructions::LDrr_ld,
        &Instructions::LDrr_le,
        &Instructions::LDrr_lh,
        &Instructions::LDrr_ll,
        &Instructions::LDrHLm_l,
        &Instructions::LDrr_la,

        // 0x70
        &Instructions::LDHLmr_b,
        &Instructions::LDHLmr_c,
        &Instructions::LDHLmr_d,
        &Instructions::LDHLmr_e,
        &Instructions::LDHLmr_h,
        &Instructions::LDHLmr_l,
        &Instructions::HALT,
        &Instructions::LDHLmr_a,
        &Instructions::LDrr_ab,
        &Instructions::LDrr_ac,
        &Instructions::LDrr_ad,
        &Instructions::LDrr_ae,
        &Instructions::LDrr_ah,
        &Instructions::LDrr_al,
        &Instructions::LDrHLm_a,
        &Instructions::LDrr_aa,

        // 0x80
        &Instructions::ADDAB,
        &Instructions::ADDAC,
        &Instructions::ADDAD,
        &Instructions::ADDAE,
        &Instructions::ADDAH,
        &Instructions::ADDAL,
        &Instructions::ADDAHLm,
        &Instructions::ADDAA,
        &Instructions::ADCAB,
        &Instructions::ADCAC,
        &Instructions::ADCAD,
        &Instructions::ADCAE,
        &Instructions::ADCAH,
        &Instructions::ADCAL,
        &Instructions::ADCAHLm,
        &Instructions::ADCAA,

        // 0x90
        &Instructions::SUBAB,
        &Instructions::SUBAC,
        &Instructions::SUBAD,
        &Instructions::SUBAE,
        &Instructions::SUBAH,
        &Instructions::SUBAL,
        &Instructions::SUBAHLm,
        &Instructions::SUBAA,
        &Instructions::SBCAB,
        &Instructions::SBCAC,
        &Instructions::SBCAD,
        &Instructions::SBCAE,
        &Instructions::SBCAH,
        &Instructions::SBCAL,
        &Instructions::SBCAHLm,
        &Instructions::SBCAA,

        // 0xA0
        &Instructions::ANDAB,
        &Instructions::ANDAC,
        &Instructions::ANDAD,
        &Instructions::ANDAE,
        &Instructions::ANDAH,
        &Instructions::ANDAL,
        &Instructions::ANDAHLm,
        &Instructions::ANDAA,
        &Instructions::XORAB,
        &Instructions::XORAC,
        &Instructions::XORAD,
        &Instructions::XORAE,
        &Instructions::XORAH,
        &Instructions::XORAL,
        &Instructions::XORAHLm,
        &Instructions::XORAA,

        // 0xB0
        &Instructions::ORAB,
        &Instructions::ORAC,
        &Instructions::ORAD,
        &Instructions::ORAE,
        &Instructions::ORAH,
        &Instructions::ORAL,
        &Instructions::ORAHLm,
        &Instructions::ORAA,
        &Instructions::CPr_b,
        &Instructions::CPr_c,
        &Instructions::CPr_d,
        &Instructions::CPr_e,
        &Instructions::CPr_h,
        &Instructions::CPr_l,
        &Instructions::CPHLm,
        &Instructions::CPr_a,

        // 0xC0
        &Instructions::RETNZ,
        &Instructions::POPBC,
        &Instructions::JPNZnn,
        &Instructions::JPnn,
        &Instructions::CALLNZnn,
        &Instructions::PUSHBC,
        &Instructions::ADDAn,
        &Instructions::RST00,
        &Instructions::RETZ,
        &Instructions::RET,
        &Instructions::JPZnn,
        &Instructions::CBInst,
        &Instructions::CALLZnn,
        &Instructions::CALLnn,
        &Instructions::ADCAn,
        &Instructions::RST08,

        // 0xD0
        &Instructions::RETNC,
        &Instructions::POPDE,
        &Instructions::JPNCnn,
        &Instructions::NOP,
        &Instructions::CALLNCnn,
        &Instructions::PUSHDE,
        &Instructions::SUBAn,
        &Instructions::RST10,
        &Instructions::RETC,
        &Instructions::RETI,
        &Instructions::JPCnn,
        &Instructions::NOP,
        &Instructions::CALLCnn,
        &Instructions::NOP,
        &Instructions::SBCAn,
        &Instructions::RST18,

        // 0xE0
        &Instructions::LDHnr_a,
        &Instructions::POPHL,
        &Instructions::LDIOCrn_a,
        &Instructions::NOP,
        &Instructions::NOP,
        &Instructions::PUSHHL,
        &Instructions::ANDAn,
        &Instructions::RST20,
        &Instructions::ADDSPn,
        &Instructions::JPHLm,
        &Instructions::LDNNmr_a,
        &Instructions::NOP,
        &Instructions::NOP,
        &Instructions::NOP,
        &Instructions::XORAn,
        &Instructions::RST28,

        // 0xF0
        &Instructions::LDHrn_a,
        &Instructions::POPAF,
        &Instructions::NOP,
        &Instructions::DI,
        &Instructions::NOP,
        &Instructions::PUSHAF,
        &Instructions::ORAn,
        &Instructions::RST30,
        &Instructions::LDHLSPn,
        &Instructions::LDSPHL,
        &Instructions::LDrNNm_a,
        &Instructions::EI,
        &Instructions::NOP,
        &Instructions::NOP,
        &Instructions::CPn,
        &Instructions::RST38
    };

    FuncPtr CBOpcodes[256] =
    {
        // 0x00
        &Instructions::CBRLCB,
        &Instructions::CBRLCC,
        &Instructions::CBRLCD,
        &Instructions::CBRLCE,
        &Instructions::CBRLCH,
        &Instructions::CBRLCL,
        &Instructions::CBRLCHLm,
        &Instructions::CBRLCA,
        &Instructions::CBRRCB,
        &Instructions::CBRRCC,
        &Instructions::CBRRCD,
        &Instructions::CBRRCE,
        &Instructions::CBRRCH,
        &Instructions::CBRRCL,
        &Instructions::CBRRCHLm,
        &Instructions::CBRRCA,

        // 0x10
        &Instructions::CBRLB,
        &Instructions::CBRLC,
        &Instructions::CBRLD,
        &Instructions::CBRLE,
        &Instructions::CBRLH,
        &Instructions::CBRLL,
        &Instructions::CBRLHLm,
        &Instructions::CBRLA,
        &Instructions::CBRRB,
        &Instructions::CBRRC,
        &Instructions::CBRRD,
        &Instructions::CBRRE,
        &Instructions::CBRRH,
        &Instructions::CBRRL,
        &Instructions::CBRRHLm,
        &Instructions::CBRRA,

        // 0x20
        &Instructions::CBSLAB,
        &Instructions::CBSLAC,
        &Instructions::CBSLAD,
        &Instructions::CBSLAE,
        &Instructions::CBSLAH,
        &Instructions::CBSLAL,
        &Instructions::CBSLAHLm,
        &Instructions::CBSLAA,
        &Instructions::CBSRAB,
        &Instructions::CBSRAC,
        &Instructions::CBSRAD,
        &Instructions::CBSRAE,
        &Instructions::CBSRAH,
        &Instructions::CBSRAL,
        &Instructions::CBSRAHLm,
        &Instructions::CBSRAA,

        // 0x30
        &Instructions::CBSWAPB,
        &Instructions::CBSWAPC,
        &Instructions::CBSWAPD,
        &Instructions::CBSWAPE,
        &Instructions::CBSWAPH,
        &Instructions::CBSWAPL,
        &Instructions::CBSWAPHLm,
        &Instructions::CBSWAPA,
        &Instructions::CBSRLB,
        &Instructions::CBSRLC,
        &Instructions::CBSRLD,
        &Instructions::CBSRLE,
        &Instructions::CBSRLH,
        &Instructions::CBSRLL,
        &Instructions::CBSRLHLm,
        &Instructions::CBSRLA,

        // 0x40
        &Instructions::CBBIT0B,
        &Instructions::CBBIT0C,
        &Instructions::CBBIT0D,
        &Instructions::CBBIT0E,
        &Instructions::CBBIT0H,
        &Instructions::CBBIT0L,
        &Instructions::CBBIT0HLm,
        &Instructions::CBBIT0A,
        &Instructions::CBBIT1B,
        &Instructions::CBBIT1C,
        &Instructions::CBBIT1D,
        &Instructions::CBBIT1E,
        &Instructions::CBBIT1H,
        &Instructions::CBBIT1L,
        &Instructions::CBBIT1HLm,
        &Instructions::CBBIT1A,

        // 0x50
        &Instructions::CBBIT2B,
        &Instructions::CBBIT2C,
        &Instructions::CBBIT2D,
        &Instructions::CBBIT2E,
        &Instructions::CBBIT2H,
        &Instructions::CBBIT2L,
        &Instructions::CBBIT2HLm,
        &Instructions::CBBIT2A,
        &Instructions::CBBIT3B,
        &Instructions::CBBIT3C,
        &Instructions::CBBIT3D,
        &Instructions::CBBIT3E,
        &Instructions::CBBIT3H,
        &Instructions::CBBIT3L,
        &Instructions::CBBIT3HLm,
        &Instructions::CBBIT3A,

        // 0x60
        &Instructions::CBBIT4B,
        &Instructions::CBBIT4C,
        &Instructions::CBBIT4D,
        &Instructions::CBBIT4E,
        &Instructions::CBBIT4H,
        &Instructions::CBBIT4L,
        &Instructions::CBBIT4HLm,
        &Instructions::CBBIT4A,
        &Instructions::CBBIT5B,
        &Instructions::CBBIT5C,
        &Instructions::CBBIT5D,
        &Instructions::CBBIT5E,
        &Instructions::CBBIT5H,
        &Instructions::CBBIT5L,
        &Instructions::CBBIT5HLm,
        &Instructions::CBBIT5A,

        // 0x70
        &Instructions::CBBIT6B,
        &Instructions::CBBIT6C,
        &Instructions::CBBIT6D,
        &Instructions::CBBIT6E,
        &Instructions::CBBIT6H,
        &Instructions::CBBIT6L,
        &Instructions::CBBIT6HLm,
        &Instructions::CBBIT6A,
        &Instructions::CBBIT7B,
        &Instructions::CBBIT7C,
        &Instructions::CBBIT7D,
        &Instructions::CBBIT7E,
        &Instructions::CBBIT7H,
        &Instructions::CBBIT7L,
        &Instructions::CBBIT7HLm,
        &Instructions::CBBIT7A,

        // 0x80
        &Instructions::CBRES0B,
        &Instructions::CBRES0C,
        &Instructions::CBRES0D,
        &Instructions::CBRES0E,
        &Instructions::CBRES0H,
        &Instructions::CBRES0L,
        &Instructions::CBRES0HLm,
        &Instructions::CBRES0A,
        &Instructions::CBRES1B,
        &Instructions::CBRES1C,
        &Instructions::CBRES1D,
        &Instructions::CBRES1E,
        &Instructions::CBRES1H,
        &Instructions::CBRES1L,
        &Instructions::CBRES1HLm,
        &Instructions::CBRES1A,

        // 0x90
        &Instructions::CBRES2B,
        &Instructions::CBRES2C,
        &Instructions::CBRES2D,
        &Instructions::CBRES2E,
        &Instructions::CBRES2H,
        &Instructions::CBRES2L,
        &Instructions::CBRES2HLm,
        &Instructions::CBRES2A,
        &Instructions::CBRES3B,
        &Instructions::CBRES3C,
        &Instructions::CBRES3D,
        &Instructions::CBRES3E,
        &Instructions::CBRES3H,
        &Instructions::CBRES3L,
        &Instructions::CBRES3HLm,
        &Instructions::CBRES3A,

        // 0xA0
        &Instructions::CBRES4B,
        &Instructions::CBRES4C,
        &Instructions::CBRES4D,
        &Instructions::CBRES4E,
        &Instructions::CBRES4H,
        &Instructions::CBRES4L,
        &Instructions::CBRES4HLm,
        &Instructions::CBRES4A,
        &Instructions::CBRES5B,
        &Instructions::CBRES5C,
        &Instructions::CBRES5D,
        &Instructions::CBRES5E,
        &Instructions::CBRES5H,
        &Instructions::CBRES5L,
        &Instructions::CBRES5HLm,
        &Instructions::CBRES5A,

        // 0xB0
        &Instructions::CBRES6B,
        &Instructions::CBRES6C,
        &Instructions::CBRES6D,
        &Instructions::CBRES6E,
        &Instructions::CBRES6H,
        &Instructions::CBRES6L,
        &Instructions::CBRES6HLm,
        &Instructions::CBRES6A,
        &Instructions::CBRES7B,
        &Instructions::CBRES7C,
        &Instructions::CBRES7D,
        &Instructions::CBRES7E,
        &Instructions::CBRES7H,
        &Instructions::CBRES7L,
        &Instructions::CBRES7HLm,
        &Instructions::CBRES7A,

        // 0xC0
        &Instructions::CBSET0B,
        &Instructions::CBSET0C,
        &Instructions::CBSET0D,
        &Instructions::CBSET0E,
        &Instructions::CBSET0H,
        &Instructions::CBSET0L,
        &Instructions::CBSET0HLm,
        &Instructions::CBSET0A,
        &Instructions::CBSET1B,
        &Instructions::CBSET1C,
        &Instructions::CBSET1D,
        &Instructions::CBSET1E,
        &Instructions::CBSET1H,
        &Instructions::CBSET1L,
        &Instructions::CBSET1HLm,
        &Instructions::CBSET1A,

        // 0xD0
        &Instructions::CBSET2B,
        &Instructions::CBSET2C,
        &Instructions::CBSET2D,
        &Instructions::CBSET2E,
        &Instructions::CBSET2H,
        &Instructions::CBSET2L,
        &Instructions::CBSET2HLm,
        &Instructions::CBSET2A,
        &Instructions::CBSET3B,
        &Instructions::CBSET3C,
        &Instructions::CBSET3D,
        &Instructions::CBSET3E,
        &Instructions::CBSET3H,
        &Instructions::CBSET3L,
        &Instructions::CBSET3HLm,
        &Instructions::CBSET3A,

        // 0xE0
        &Instructions::CBSET4B,
        &Instructions::CBSET4C,
        &Instructions::CBSET4D,
        &Instructions::CBSET4E,
        &Instructions::CBSET4H,
        &Instructions::CBSET4L,
        &Instructions::CBSET4HLm,
        &Instructions::CBSET4A,
        &Instructions::CBSET5B,
        &Instructions::CBSET5C,
        &Instructions::CBSET5D,
        &Instructions::CBSET5E,
        &Instructions::CBSET5H,
        &Instructions::CBSET5L,
        &Instructions::CBSET5HLm,
        &Instructions::CBSET5A,

        // 0xF0
        &Instructions::CBSET6B,
        &Instructions::CBSET6C,
        &Instructions::CBSET6D,
        &Instructions::CBSET6E,
        &Instructions::CBSET6H,
        &Instructions::CBSET6L,
        &Instructions::CBSET6HLm,
        &Instructions::CBSET6A,
        &Instructions::CBSET7B,
        &Instructions::CBSET7C,
        &Instructions::CBSET7D,
        &Instructions::CBSET7E,
        &Instructions::CBSET7H,
        &Instructions::CBSET7L,
        &Instructions::CBSET7HLm,
        &Instructions::CBSET7A,
    };
};

#endif // Z80INSTRUCTIONS_H
