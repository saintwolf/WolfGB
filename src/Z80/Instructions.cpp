#include "Instructions.h"

#include <stdio.h>

Instructions::Instructions(Registers* registers, MMU* mmu)
{
    this->registers = registers;
    this->mmu = mmu;
}

Instructions::~Instructions()
{
}

void Instructions::ExecuteInstruction(uint8_t opcode)
{
    (this->*OpCodes[opcode])();
}

uint8_t Instructions::LoadImmediate8()
{
    return mmu->ReadByte(registers->pc++);
}

uint16_t Instructions::LoadImmediate16()
{
    uint8_t low = mmu->ReadByte(registers->pc++);
    uint8_t high = mmu->ReadByte(registers->pc++);
    return low + (high << 8);
}

void Instructions::PushStack(uint16_t data)
{
    registers->sp -= 2;
    mmu->WriteWord(registers->sp, data);
}

uint16_t Instructions::PopStack()
{
    uint16_t data = mmu->ReadWord(registers->sp);
    registers->sp += 2;
    return data;
}

/** @brief Checks for half carry, sets appropriate flag
     *
     * @param a uint8_t The first operand
     * @param b uint8_t The second operand
     * @param carryIn bool Sets carry-in bit
     * @return void
     *
*/
void Instructions::CheckHalfCarry(uint8_t a, uint8_t b, bool carryIn)
{
    // Adds a, b and carry in lower nibble to see if it sets bit 4
    if ((a & 0xF) + (b & 0xF) + (int)carryIn & 0x10)
    {
        registers->SetFlag(Flags::H);
    }
    else
    {
        registers->ClearFlag(Flags::H);
    }
}

/** @brief Checks for half carry (bit 11), sets appropriate flag
     *
     * @param a uint16_t The first operand
     * @param b uint16_t The second operand
     * @param carryIn bool Sets carry-in
     * @return void
     *
*/
void Instructions::CheckHalfCarry(uint16_t a, uint16_t b, bool carryIn)
{
    // Adds a, b and carry in lower byte to see if it sets bit 8
    if ((a & 0xFFF) + (b & 0xFFF) + carryIn & 0xF000)
    {
        registers->SetFlag(Flags::H);
    }
    else
    {
        registers->ClearFlag(Flags::H);
    }
}

/** @brief Checks for carry, sets appropriate flag
     *
     * @param a uint8_t The first operand
     * @param b uint8_t The second operand
     * @param carryIn bool Sets carry-in bit
     * @return void
     *
*/
void Instructions::CheckCarry(uint8_t a, uint8_t b, bool carryIn)
{
    b += carryIn;
    CheckHalfCarry(a, b);
    if ((uint16_t)(a + b) & 0x100)
    {
        registers->SetFlag(Flags::C);
    }
    else
    {
        registers->ClearFlag(Flags::C);
    }
}

/** @brief Checks for carry (bit 15), sets appropriate flag
     *
     * @param a uint16_t The first operand
     * @param b uint16_t The second operand
     * @param carryIn bool Sets carry-in
     * @return void
     *
*/
void Instructions::CheckCarry(uint16_t a, uint16_t b, bool carryIn)
{
    b += carryIn;
    CheckHalfCarry(a, b);
    if ((uint32_t)(a & 0xFFFF) + (b & 0xFFFF) + (int) carryIn & 0x10000)
    {
        registers->SetFlag(Flags::C);
    }
    else
    {
        registers->ClearFlag(Flags::C);
    }
}

/** @brief Checks for borrow from bit 4, sets flag if there is NO borrow
     *
     * @param a uint8_t The first operand
     * @param b uint8_t The second operand
     * @param carryIn bool Sets the carry-in
     * @return void
     *
*/
void Instructions::CheckHalfBorrow(uint8_t a, uint8_t b, bool carryIn)
{
    b += carryIn;
    if ((a & 0x0F) < (b & 0x0F))
    {
        registers->ClearFlag(Flags::H);
    }
    else
    {
        registers->SetFlag(Flags::H);
    }
}

/** @brief Checks for borrow, sets appropriate flags
     *
     * @param a uint8_t The first operand
     * @param b uint8_t The second operand
     * @param carryIn bool Sets the carry-in
     * @return void
     *
*/
void Instructions::CheckBorrow(uint8_t a, uint8_t b, bool carryIn)
{
    b += carryIn;
    CheckHalfBorrow(a, b);
    // Set carry flag if a is smaller than b + carry
    if (a < b + (uint8_t) carryIn)
    {
        registers->SetFlag(Flags::C);
    }
    else
    {
        registers->ClearFlag(Flags::C);
    }
}

/** @brief Checks if the result is 0 and sets appropriate flags
     *
     * @param result uint8_t The result of the operation
     * @return void
     *
*/
void Instructions::CheckZero(uint8_t result)
{
    if (result == 0)
    {
        registers->SetFlag(Flags::Z);
    }
    else
    {
        registers->ClearFlag(Flags::Z);
    }
}

// A group of functions to reduce duplicated code for some of the opcodes.

void Instructions::LDrn(uint8_t& reg)
{
    reg = LoadImmediate8();
}

void Instructions::LDrr(uint8_t& dest, uint8_t& source)
{
    dest = source;
}

void Instructions::LDrRRm(uint8_t& dest, uint16_t& source)
{
    dest = mmu->ReadByte(source);
}

void Instructions::LDRRmr(uint16_t& dest, uint8_t& source)
{
    mmu->WriteByte(dest, source);
}

void Instructions::PUSHRR(uint16_t& reg)
{
    PushStack(reg);
}

void Instructions::POPRR(uint16_t& reg)
{
    reg = PopStack();
}

void Instructions::ADDAr(uint8_t& reg)
{
    registers->ClearFlag(Flags::N);
    CheckCarry(registers->a, reg);
    registers->a += reg;
    CheckZero(registers->a);
}

void Instructions::ADCAr(uint8_t& reg)
{
    bool carry = false;
    if (registers->GetFlag(Flags::C) > 0)
    {
        carry = true;
    }
    registers->ClearFlag(Flags::N);
    CheckCarry(registers->a, reg, carry);
    registers->a = reg + (uint8_t) carry;
    CheckZero(registers->a);
}

void Instructions::SUBAr(uint8_t& reg)
{
    registers->SetFlag(Flags::N);
    CheckBorrow(registers->a, reg);
    registers->a -= reg;
    CheckZero(registers->a);
}

void Instructions::SBCAr(uint8_t& reg)
{
    bool carry = false;
    if (registers->GetFlag(Flags::C) > 0)
    {
        carry = true;
    }
    registers->SetFlag(Flags::N);
    CheckBorrow(registers->a, reg, carry);
    registers->a -= reg + (uint8_t)carry;
    CheckZero(registers->a);
}

void Instructions::ANDAr(uint8_t& reg)
{
    registers->ClearFlag(Flags::N);
    registers->ClearFlag(Flags::C);
    registers->SetFlag(Flags::H);
    registers->a &= reg;
    CheckZero(registers->a);
}

void Instructions::ORAr(uint8_t& reg)
{
    registers->ClearFlag(Flags::N);
    registers->ClearFlag(Flags::H);
    registers->ClearFlag(Flags::C);
    registers->a |= reg;
    CheckZero(registers->a);
}

void Instructions::XORAr(uint8_t& reg)
{
    registers->ClearFlag(Flags::N);
    registers->ClearFlag(Flags::H);
    registers->ClearFlag(Flags::C);
    registers->a ^= reg;
    CheckZero(registers->a);
}

void Instructions::CPr(uint8_t& reg)
{
    registers->SetFlag(Flags::N);
    CheckBorrow(registers->a, reg);
    if (reg == registers->a)
    {
        registers->SetFlag(Flags::Z);
    }
    else
    {
        registers->ClearFlag(Flags::Z);
    }
}

void Instructions::INCr(uint8_t& reg)
{
    registers->ClearFlag(Flags::N);
    CheckCarry(reg, 1, false);
    CheckZero(++reg);
}

void Instructions::DECr(uint8_t& reg)
{
    registers->SetFlag(Flags::N);
    CheckBorrow(reg, 1, false);
    CheckZero(--reg);
}

void Instructions::ADDHLRR(uint16_t& reg)
{
    registers->ClearFlag(Flags::N);
    CheckCarry(registers->hl, reg, false);
    registers->hl += reg;
}

void Instructions::INCRR(uint16_t& reg)
{
    (reg)++;
}

void Instructions::DECRR(uint16_t& reg)
{
    (reg)--;
}

void Instructions::RSTN(uint8_t offset)
{
    PushStack(registers->pc);
    registers->pc = 0 + offset;
}

/** @brief Shifts register one left. 7th bit is moved to carry flag, and bit 0
 *
 * @param reg uint8_t& The register
 * @return void
 *
 */
void Instructions::CBRLCr(uint8_t& reg)
{
    registers->ClearFlag(Flags::N);
    registers->ClearFlag(Flags::H);
    int carry = reg >> 7 & 0x1; // Set Carry to the 7th bit of reg, and shift to LSB
    reg <<= 1;
    reg |= carry; // Add the carry to the 0th bit
    carry ? registers->SetFlag(Flags::C) : registers->ClearFlag(Flags::C); // Set carry flag as per carry var
    CheckZero(reg);
}

/** @brief Shifts register one left. 7th bit moved to carry flag, carry flag moved to bit 0.
 *
 * @param reg uint8_t& The register
 * @return void
 *
 */
void Instructions::CBRLr(uint8_t& reg)
{
    registers->ClearFlag(Flags::N);
    registers->ClearFlag(Flags::H);
    int carry = reg >> 7 & 0x1;
    reg <<= 1;
    registers->GetFlag(Flags::C) ? reg |= 0x01 : reg |= 0x00; // Set 0th bit to carry flag
    carry ? registers->SetFlag(Flags::C) : registers->ClearFlag(Flags::C); // Set carry flag as per carry var
    CheckZero(reg);
}

/** @brief Shifts register one right. 0th bit is moved to carry flag and bit 7
 *
 * @param reg uint8_t& The register
 * @return void
 *
 */
void Instructions::CBRRCr(uint8_t& reg)
{
    registers->ClearFlag(Flags::N);
    registers->ClearFlag(Flags::H);
    int carry = reg << 7 & 0x80;
    reg >>= 1;
    reg |= carry; // Add the carry to the 7th bit
    carry ? registers->SetFlag(Flags::C) : registers->ClearFlag(Flags::C); // Set carry flag as per carry var
    CheckZero(reg);
}

/** @brief Shifts register one right. 0th bit is moved to carry flag, carry is moved to bit 7
 *
 * @param reg uint8_t& The register
 * @return void
 *
 */
void Instructions::CBRRr(uint8_t& reg)
{
    registers->ClearFlag(Flags::N);
    registers->ClearFlag(Flags::H);
    int carry = reg << 7 & 0x80; // Save bit 0, to move to carry
    reg >>= 1;
    registers->GetFlag(Flags::C) ? reg |= 0x80 : reg |= 0x00; // Set 7th bit to carry flag // Set 7th bit to carry
    carry ? registers->SetFlag(Flags::C) : registers->ClearFlag(Flags::C); // Set carry flag as per carry var
    CheckZero(reg);
}

/** @brief Shifts register one left. 7th bit is moved to carry flag
 *
 * @param reg uint8_t* The register
 * @return void
 *
 */
void Instructions::CBSLAr(uint8_t& reg)
{
    registers->ClearFlag(Flags::N);
    registers->ClearFlag(Flags::H);
    reg & 0x80 ? registers->SetFlag(Flags::C) : registers->ClearFlag(Flags::C); // Set carry flag to bit 7
    reg <<= 1;
    CheckZero(reg);
}

/** @brief Shifts register one right. 0th bit is moves to carry flag
 *
 * @param reg uint8_t* The register
 * @return void
 *
 */
void Instructions::CBSRAr(uint8_t& reg)
{
    registers->ClearFlag(Flags::N);
    registers->ClearFlag(Flags::H);
    reg & 0x1 ? registers->SetFlag(Flags::C) : registers->ClearFlag(Flags::C); // Set carry flag to bit 7
    reg >>= 1;
    CheckZero(reg);
}

/** @brief The contents of reg are shifted right 1 bit position
 * The contents of reg are shifted right 1 bit position.
 * Bit 0 is copied to the carry flag, bit 7 is reset.
 *
 * @param reg uint8_t* The register
 * @return void
 *
 */
void Instructions::CBSRLr(uint8_t& reg)
{
    registers->ClearFlag(Flags::N);
    registers->ClearFlag(Flags::H);
    reg & 0x1 ? registers->SetFlag(Flags::C) : registers->ClearFlag(Flags::C); // Set carry flag to bit 7
    reg >>= 1;
    reg &= 0x80; // Reset bit 7, just in case
    CheckZero(reg);
}

/** @brief Test bit b in register reg and sets Z flag accordingly
 *
 * @param b uint8_t
 * @param reg uint8_t*
 * @return void
 *
 */
void Instructions::CBBITbr(uint8_t b, uint8_t& reg)
{
    registers->SetFlag(Flags::H);
    registers->ClearFlag(Flags::N);
    CheckZero(reg >> b & 1);
}

/** @brief Set bit b in register reg. No flags affected.
 *
 * @param b uint8_t
 * @param reg uint8_t*
 * @return void
 *
 */
void Instructions::CBSETbr(uint8_t b, uint8_t& reg)
{
    reg |= 1 << b;
}

/** @brief Reset bit b in register reg. No flags affected.
 *
 * @param b uint8_t
 * @param reg uint8_t*
 * @return void
 *
 */
void Instructions::CBRESbr(uint8_t b, uint8_t& reg)
{
    reg &= ~(1 << b);
}

/** @brief Swap upper and lower nibbles of n
 *
 * @param reg uint8_t*
 * @return void
 *
 */
void Instructions::CBSWAPn(uint8_t& reg)
{
    registers->ClearFlag(Flags::N);
    registers->ClearFlag(Flags::H);
    registers->ClearFlag(Flags::C);

    uint8_t upper = reg >> 4;
    reg <<= 4;
    reg += upper;

    if (reg == 0)
        registers->SetFlag(Flags::Z);
    else
        registers->ClearFlag(Flags::Z);
}

//***********************//
//***** 8-Bit Loads *****//
//***********************//

int Instructions::LDrn_a()
{
    LDrn(registers->a);
    return 0;
}
int Instructions::LDrn_b()
{
    LDrn(registers->b);
    return 0;
}
int Instructions::LDrn_c()
{
    LDrn(registers->c);
    return 0;
}
int Instructions::LDrn_d()
{
    LDrn(registers->d);
    return 0;
}
int Instructions::LDrn_e()
{
    LDrn(registers->e);
    return 0;
}
int Instructions::LDrn_h()
{
    LDrn(registers->h);
    return 0;
}
int Instructions::LDrn_l()
{
    LDrn(registers->l);
    return 0;
}

int Instructions::LDrr_aa()
{
    LDrr(registers->a, registers->a);
    return 0;
}
int Instructions::LDrr_ab()
{
    LDrr(registers->a, registers->b);
    return 0;
}
int Instructions::LDrr_ac()
{
    LDrr(registers->a, registers->c);
    return 0;
}
int Instructions::LDrr_ad()
{
    LDrr(registers->a, registers->d);
    return 0;
}
int Instructions::LDrr_ae()
{
    LDrr(registers->a, registers->e);
    return 0;
}
int Instructions::LDrr_ah()
{
    LDrr(registers->a, registers->h);
    return 0;
}
int Instructions::LDrr_al()
{
    LDrr(registers->a, registers->l);
    return 0;
}
int Instructions::LDrr_ba()
{
    LDrr(registers->b, registers->a);
    return 0;
}
int Instructions::LDrr_bb()
{
    LDrr(registers->b, registers->b);
    return 0;
}
int Instructions::LDrr_bc()
{
    LDrr(registers->b, registers->c);
    return 0;
}
int Instructions::LDrr_bd()
{
    LDrr(registers->b, registers->d);
    return 0;
}
int Instructions::LDrr_be()
{
    LDrr(registers->b, registers->e);
    return 0;
}
int Instructions::LDrr_bh()
{
    LDrr(registers->b, registers->h);
    return 0;
}
int Instructions::LDrr_bl()
{
    LDrr(registers->b, registers->l);
    return 0;
}
int Instructions::LDrr_ca()
{
    LDrr(registers->c, registers->a);
    return 0;
}
int Instructions::LDrr_cb()
{
    LDrr(registers->c, registers->b);
    return 0;
}
int Instructions::LDrr_cc()
{
    LDrr(registers->c, registers->c);
    return 0;
}
int Instructions::LDrr_cd()
{
    LDrr(registers->c, registers->d);
    return 0;
}
int Instructions::LDrr_ce()
{
    LDrr(registers->c, registers->e);
    return 0;
}
int Instructions::LDrr_ch()
{
    LDrr(registers->c, registers->h);
    return 0;
}
int Instructions::LDrr_cl()
{
    LDrr(registers->c, registers->l);
    return 0;
}
int Instructions::LDrr_da()
{
    LDrr(registers->d, registers->a);
    return 0;
}
int Instructions::LDrr_db()
{
    LDrr(registers->d, registers->b);
    return 0;
}
int Instructions::LDrr_dc()
{
    LDrr(registers->d, registers->c);
    return 0;
}
int Instructions::LDrr_dd()
{
    LDrr(registers->d, registers->d);
    return 0;
}
int Instructions::LDrr_de()
{
    LDrr(registers->d, registers->e);
    return 0;
}
int Instructions::LDrr_dh()
{
    LDrr(registers->d, registers->h);
    return 0;
}
int Instructions::LDrr_dl()
{
    LDrr(registers->d, registers->l);
    return 0;
}
int Instructions::LDrr_ea()
{
    LDrr(registers->e, registers->a);
    return 0;
}
int Instructions::LDrr_eb()
{
    LDrr(registers->e, registers->b);
    return 0;
}
int Instructions::LDrr_ec()
{
    LDrr(registers->e, registers->c);
    return 0;
}
int Instructions::LDrr_ed()
{
    LDrr(registers->e, registers->d);
    return 0;
}
int Instructions::LDrr_ee()
{
    LDrr(registers->e, registers->e);
    return 0;
}
int Instructions::LDrr_eh()
{
    LDrr(registers->e, registers->h);
    return 0;
}
int Instructions::LDrr_el()
{
    LDrr(registers->e, registers->l);
    return 0;
}
int Instructions::LDrr_ha()
{
    LDrr(registers->h, registers->a);
    return 0;
}
int Instructions::LDrr_hb()
{
    LDrr(registers->h, registers->b);
    return 0;
}
int Instructions::LDrr_hc()
{
    LDrr(registers->h, registers->c);
    return 0;
}
int Instructions::LDrr_hd()
{
    LDrr(registers->h, registers->d);
    return 0;
}
int Instructions::LDrr_he()
{
    LDrr(registers->h, registers->e);
    return 0;
}
int Instructions::LDrr_hh()
{
    LDrr(registers->h, registers->h);
    return 0;
}
int Instructions::LDrr_hl()
{
    LDrr(registers->h, registers->l);
    return 0;
}
int Instructions::LDrr_la()
{
    LDrr(registers->l, registers->a);
    return 0;
}
int Instructions::LDrr_lb()
{
    LDrr(registers->l, registers->b);
    return 0;
}
int Instructions::LDrr_lc()
{
    LDrr(registers->l, registers->c);
    return 0;
}
int Instructions::LDrr_ld()
{
    LDrr(registers->l, registers->d);
    return 0;
}
int Instructions::LDrr_le()
{
    LDrr(registers->l, registers->e);
    return 0;
}
int Instructions::LDrr_lh()
{
    LDrr(registers->l, registers->h);
    return 0;
}
int Instructions::LDrr_ll()
{
    LDrr(registers->l, registers->l);
    return 0;
}

int Instructions::LDrBCm_a()
{
    LDrRRm(registers->a, registers->bc);
    return 0;
}
int Instructions::LDrDEm_a()
{
    LDrRRm(registers->a, registers->de);
    return 0;
}
int Instructions::LDrHLm_a()
{
    LDrRRm(registers->a, registers->hl);
    return 0;
}
int Instructions::LDrHLm_b()
{
    LDrRRm(registers->b, registers->hl);
    return 0;
}
int Instructions::LDrHLm_c()
{
    LDrRRm(registers->c, registers->hl);
    return 0;
}
int Instructions::LDrHLm_d()
{
    LDrRRm(registers->d, registers->hl);
    return 0;
}
int Instructions::LDrHLm_e()
{
    LDrRRm(registers->e, registers->hl);
    return 0;
}
int Instructions::LDrHLm_h()
{
    LDrRRm(registers->h, registers->hl);
    return 0;
}
int Instructions::LDrHLm_l()
{
    LDrRRm(registers->l, registers->hl);
    return 0;
}

int Instructions::LDBCmr_a()
{
    LDRRmr(registers->bc, registers->a);
    return 0;
}
int Instructions::LDDEmr_a()
{
    LDRRmr(registers->de, registers->a);
    return 0;
}
int Instructions::LDHLmr_a()
{
    LDRRmr(registers->hl, registers->a);
    return 0;
}
int Instructions::LDHLmr_b()
{
    LDRRmr(registers->hl, registers->b);
    return 0;
}
int Instructions::LDHLmr_c()
{
    LDRRmr(registers->hl, registers->c);
    return 0;
}
int Instructions::LDHLmr_d()
{
    LDRRmr(registers->hl, registers->d);
    return 0;
}
int Instructions::LDHLmr_e()
{
    LDRRmr(registers->hl, registers->e);
    return 0;
}
int Instructions::LDHLmr_h()
{
    LDRRmr(registers->hl, registers->h);
    return 0;
}
int Instructions::LDHLmr_l()
{
    LDRRmr(registers->hl, registers->l);
    return 0;
}

int Instructions::LDHLmn()
{
    mmu->WriteByte(registers->hl, LoadImmediate8());
    return 0;
}
int Instructions::LDNNmr_a()
{
    mmu->WriteByte(mmu->ReadByte(LoadImmediate16()), registers->a);
    return 0;
}
int Instructions::LDrNNm_a()
{
    registers->a = mmu->ReadByte(LoadImmediate8());
    return 0;
}

int Instructions::LDDHLmr_a()
{
    mmu->WriteByte(registers->hl--, registers->a);
    return 0;
}

int Instructions::LDDrHLm_a()
{
    uint8_t HLm = mmu->GetMemoryRef(registers->hl);
    LDrr(registers->a, HLm);
    registers->hl--;
    return 0;
}

int Instructions::LDIHLmr_a()
{
    uint8_t HLm = mmu->GetMemoryRef(registers->hl);
    LDrr(HLm, registers->a);
    registers->hl++;
    return 0;
}

int Instructions::LDIrHLm_a()
{
    uint8_t HLm = mmu->GetMemoryRef(registers->hl);
    LDrr(registers->a, HLm);
    registers->hl++;
    return 0;
}

// IO Loads
int Instructions::LDHrn_a()
{
    registers->a = mmu->ReadByte(0xFF00 + LoadImmediate8());
    return 0;
}
int Instructions::LDHnr_a()
{
    mmu->WriteByte(0xFF00 + LoadImmediate8(), registers->a);
    return 0;
}

int Instructions::LDIOCrn_a()
{
    registers->a = mmu->ReadByte(0xFF00 + registers->c);
    return 0;
}
int Instructions::LDIOCnr_a()
{
    mmu->WriteByte(0xFF00 + registers->c, registers->a);
    return 0;
}


//************************//
//***** 16-Bit Loads *****//
//************************//

int Instructions::LDBCnn()
{
    registers->bc = LoadImmediate16();
    return 0;
}
int Instructions::LDDEnn()
{
    registers->de = LoadImmediate16();
    return 0;
}
int Instructions::LDHLnn()
{
    registers->hl = LoadImmediate16();
    return 0;
}
int Instructions::LDSPnn()
{
    registers->sp = LoadImmediate16();
    return 0;
}

int Instructions::LDSPHL()
{
    registers->sp = registers->hl;
    return 0;
}
int Instructions::LDHLSPn()
{
    registers->hl = registers->sp + LoadImmediate16();
    return 0;
}
int Instructions::LDNNmSP()
{
    registers->sp = LoadImmediate16();
    return 0;
}

int Instructions::PUSHAF()
{
    PUSHRR(registers->af);
    return 0;
}
int Instructions::PUSHBC()
{
    PUSHRR(registers->bc);
    return 0;
}
int Instructions::PUSHDE()
{
    PUSHRR(registers->de);
    return 0;
}
int Instructions::PUSHHL()
{
    PUSHRR(registers->hl);
    return 0;
}

int Instructions::POPAF()
{
    POPRR(registers->af);
    return 0;
}
int Instructions::POPBC()
{
    POPRR(registers->bc);
    return 0;
}
int Instructions::POPDE()
{
    POPRR(registers->de);
    return 0;
}
int Instructions::POPHL()
{
    POPRR(registers->hl);
    return 0;
}


//*********************//
//***** 8-Bit ALU *****//
//*********************//

int Instructions::ADDAA()
{
    ADDAr(registers->a);
    return 0;
}
int Instructions::ADDAB()
{
    ADDAr(registers->b);
    return 0;
}
int Instructions::ADDAC()
{
    ADDAr(registers->c);
    return 0;
}
int Instructions::ADDAD()
{
    ADDAr(registers->d);
    return 0;
}
int Instructions::ADDAE()
{
    ADDAr(registers->e);
    return 0;
}
int Instructions::ADDAH()
{
    ADDAr(registers->h);
    return 0;
}
int Instructions::ADDAL()
{
    ADDAr(registers->l);
    return 0;
}
int Instructions::ADDAHLm()
{
    uint8_t value = mmu->ReadByte(registers->hl);
    ADDAr(value);
    return 0;
}
int Instructions::ADDAn()
{
    uint8_t value = LoadImmediate8();
    return 0;
    ADDAr(value);
    return 0;
    return 0;
}

int Instructions::ADCAA()
{
    ADCAr(registers->a);
    return 0;
}
int Instructions::ADCAB()
{
    ADCAr(registers->b);
    return 0;
}
int Instructions::ADCAC()
{
    ADCAr(registers->c);
    return 0;
}
int Instructions::ADCAD()
{
    ADCAr(registers->d);
    return 0;
}
int Instructions::ADCAE()
{
    ADCAr(registers->e);
    return 0;
}
int Instructions::ADCAH()
{
    ADCAr(registers->h);
    return 0;
}
int Instructions::ADCAL()
{
    ADCAr(registers->l);
    return 0;
}
int Instructions::ADCAHLm()
{
    uint8_t value = mmu->ReadByte(registers->hl);
    ADCAr(value);
    return 0;
}
int Instructions::ADCAn()
{
    uint8_t value = LoadImmediate8();
    return 0;
    ADCAr(value);
    return 0;
    return 0;
}

int Instructions::SUBAA()
{
    SUBAr(registers->a);
    return 0;
}
int Instructions::SUBAB()
{
    SUBAr(registers->b);
    return 0;
}
int Instructions::SUBAC()
{
    SUBAr(registers->c);
    return 0;
}
int Instructions::SUBAD()
{
    SUBAr(registers->d);
    return 0;
}
int Instructions::SUBAE()
{
    SUBAr(registers->e);
    return 0;
}
int Instructions::SUBAH()
{
    SUBAr(registers->h);
    return 0;
}
int Instructions::SUBAL()
{
    SUBAr(registers->l);
    return 0;
}
int Instructions::SUBAHLm()
{
    uint8_t value = mmu->ReadByte(registers->hl);
    SUBAr(value);
    return 0;
}
int Instructions::SUBAn()
{
    uint8_t value = LoadImmediate8();
    return 0;
    SUBAr(value);
    return 0;
    return 0;
}

int Instructions::SBCAA()
{
    SBCAr(registers->a);
    return 0;
}
int Instructions::SBCAB()
{
    SBCAr(registers->b);
    return 0;
}
int Instructions::SBCAC()
{
    SBCAr(registers->c);
    return 0;
}
int Instructions::SBCAD()
{
    SBCAr(registers->d);
    return 0;
}
int Instructions::SBCAE()
{
    SBCAr(registers->e);
    return 0;
}
int Instructions::SBCAH()
{
    SBCAr(registers->h);
    return 0;
}
int Instructions::SBCAL()
{
    SBCAr(registers->l);
    return 0;
}
int Instructions::SBCAHLm()
{
    uint8_t value = mmu->ReadByte(registers->hl);
    SBCAr(value);
    return 0;
}
int Instructions::SBCAn()
{
    uint8_t value = LoadImmediate8();
    return 0;
    SBCAr(value);
    return 0;
    return 0;
}

int Instructions::ANDAA()
{
    ANDAr(registers->a);
    return 0;
}
int Instructions::ANDAB()
{
    ANDAr(registers->b);
    return 0;
}
int Instructions::ANDAC()
{
    ANDAr(registers->c);
    return 0;
}
int Instructions::ANDAD()
{
    ANDAr(registers->d);
    return 0;
}
int Instructions::ANDAE()
{
    ANDAr(registers->e);
    return 0;
}
int Instructions::ANDAH()
{
    ANDAr(registers->h);
    return 0;
}
int Instructions::ANDAL()
{
    ANDAr(registers->l);
    return 0;
}
int Instructions::ANDAHLm()
{
    uint8_t value = mmu->ReadByte(registers->hl);
    ANDAr(value);
    return 0;
}
int Instructions::ANDAn()
{
    uint8_t value = LoadImmediate8();
    return 0;
    ANDAr(value);
    return 0;
    return 0;
}

int Instructions::ORAA()
{
    ORAr(registers->a);
    return 0;
}
int Instructions::ORAB()
{
    ORAr(registers->b);
    return 0;
}
int Instructions::ORAC()
{
    ORAr(registers->c);
    return 0;
}
int Instructions::ORAD()
{
    ORAr(registers->d);
    return 0;
}
int Instructions::ORAE()
{
    ORAr(registers->e);
    return 0;
}
int Instructions::ORAH()
{
    ORAr(registers->h);
    return 0;
}
int Instructions::ORAL()
{
    ORAr(registers->l);
    return 0;
}
int Instructions::ORAHLm()
{
    uint8_t value = mmu->ReadByte(registers->hl);
    ORAr(value);
    return 0;
}
int Instructions::ORAn()
{
    uint8_t value = LoadImmediate8();
    return 0;
    ORAr(value);
    return 0;
    return 0;
}

int Instructions::XORAA()
{
    XORAr(registers->a);
    return 0;
}
int Instructions::XORAB()
{
    XORAr(registers->b);
    return 0;
}
int Instructions::XORAC()
{
    XORAr(registers->c);
    return 0;
}
int Instructions::XORAD()
{
    XORAr(registers->d);
    return 0;
}
int Instructions::XORAE()
{
    XORAr(registers->e);
    return 0;
}
int Instructions::XORAH()
{
    XORAr(registers->h);
    return 0;
}
int Instructions::XORAL()
{
    XORAr(registers->l);
    return 0;
}
int Instructions::XORAHLm()
{
    uint8_t value = mmu->ReadByte(registers->hl);
    XORAr(value);
    return 0;
}
int Instructions::XORAn()
{
    uint8_t value = LoadImmediate8();
    return 0;
    XORAr(value);
    return 0;
    return 0;
}

int Instructions::CPr_a()
{
    CPr(registers->a);
    return 0;
}
int Instructions::CPr_b()
{
    CPr(registers->b);
    return 0;
}
int Instructions::CPr_c()
{
    CPr(registers->c);
    return 0;
}
int Instructions::CPr_d()
{
    CPr(registers->d);
    return 0;
}
int Instructions::CPr_e()
{
    CPr(registers->e);
    return 0;
}
int Instructions::CPr_h()
{
    CPr(registers->h);
    return 0;
}
int Instructions::CPr_l()
{
    CPr(registers->l);
    return 0;
}
int Instructions::CPHLm()
{
    uint8_t value = mmu->ReadByte(registers->hl);
    CPr(value);
    return 0;
}
int Instructions::CPn()
{
    uint8_t value = LoadImmediate8();
    return 0;
    CPr(value);
    return 0;
    return 0;
}

int Instructions::INCA()
{
    INCr(registers->a);
    return 0;
}
int Instructions::INCB()
{
    INCr(registers->b);
    return 0;
}
int Instructions::INCC()
{
    INCr(registers->c);
    return 0;
}
int Instructions::INCD()
{
    INCr(registers->d);
    return 0;
}
int Instructions::INCE()
{
    INCr(registers->e);
    return 0;
}
int Instructions::INCH()
{
    INCr(registers->h);
    return 0;
}
int Instructions::INCL()
{
    INCr(registers->l);
    return 0;
}
int Instructions::INCHLm()
{
    uint8_t value = mmu->ReadByte(registers->hl);
    return 0;
    INCr(value);
    return 0;
    return 0;
}

int Instructions::DECA()
{
    DECr(registers->a);
    return 0;
}
int Instructions::DECB()
{
    DECr(registers->b);
    return 0;
}
int Instructions::DECC()
{
    DECr(registers->c);
    return 0;
}
int Instructions::DECD()
{
    DECr(registers->d);
    return 0;
}
int Instructions::DECE()
{
    DECr(registers->e);
    return 0;
}
int Instructions::DECH()
{
    DECr(registers->h);
    return 0;
}
int Instructions::DECL()
{
    DECr(registers->l);
    return 0;
}
int Instructions::DECHLm()
{
    uint8_t value = mmu->ReadByte(registers->hl);
    return 0;
    DECr(value);
    return 0;
    return 0;
}

//**********************//
//***** 16-Bit ALU *****//
//**********************//

int Instructions::ADDHLBC()
{
    ADDHLRR(registers->bc);
    return 0;
}
int Instructions::ADDHLDE()
{
    ADDHLRR(registers->de);
    return 0;
}
int Instructions::ADDHLHL()
{
    ADDHLRR(registers->hl);
    return 0;
}
int Instructions::ADDHLSP()
{
    ADDHLRR(registers->sp);
    return 0;
}

int Instructions::ADDSPn()
{
    registers->sp += LoadImmediate8();
    return 0;
}

int Instructions::INCBC()
{
    INCRR(registers->bc);
    return 0;
}
int Instructions::INCDE()
{
    INCRR(registers->de);
    return 0;
}
int Instructions::INCHL()
{
    INCRR(registers->hl);
    return 0;
}
int Instructions::INCSP()
{
    INCRR(registers->sp);
    return 0;
}

int Instructions::DECBC()
{
    DECRR(registers->bc);
    return 0;
}
int Instructions::DECDE()
{
    DECRR(registers->de);
    return 0;
}
int Instructions::DECHL()
{
    DECRR(registers->hl);
    return 0;
}
int Instructions::DECSP()
{
    DECRR(registers->sp);
    return 0;
}

//****************//
//***** MISC *****//
//****************//

int Instructions::DAA()
{
    int a = registers->a;

    if (!registers->GetFlag(Flags::N))
    {
        if (registers->GetFlag(Flags::H) || (a & 0xF) > 9)
            a += 0x06;
        if (registers->GetFlag(Flags::C) || a > 0x9F)
            a += 0x60;
    }
    else
    {
        if (registers->GetFlag(Flags::H))
            a = (a - 6) & 0xFF;

        if (registers->GetFlag(Flags::C))
            a -= 0x60;
    }

    registers->ClearFlag(Flags::H);
    registers->ClearFlag(Flags::Z);

    if ((a & 0x100) == 0x100)
        registers->SetFlag(Flags::C);

    a &= 0xFF;

    if (a == 0)
        registers->SetFlag(Flags::Z);

    registers->a = a;

    return 0;
}

int Instructions::CPL()
{
    registers->ClearFlag(Flags::N);
    registers->ClearFlag(Flags::H);

    registers->a = ~registers->a;
    return 0;
}

int Instructions::CCF()
{
    registers->ClearFlag(Flags::N);
    registers->ClearFlag(Flags::H);

    if (registers->GetFlag(Flags::C))
        registers->ClearFlag(Flags::C);
    else
        registers->SetFlag(Flags::C);

    return 0;
}

int Instructions::SCF()
{
    registers->ClearFlag(Flags::N);
    registers->ClearFlag(Flags::H);
    registers->SetFlag(Flags::C);

    return 0;
}

int Instructions::HALT()
{
    // @todo Interrupts
    return 0;
}

int Instructions::STOP()
{
    // @todo Interrupts
    return 0;
}

int Instructions::DI()
{
    // @todo Interrupts
    return 0;
}

int Instructions::EI()
{
    // @todo Interrupts
    return 0;
}

//******************************//
//***** Rotates and Shifts *****//
//******************************//

int Instructions::RLCA()
{
    CBRLCA();
    return 0;
}
int Instructions::RLA()
{
    CBRLA();
    return 0;
}
int Instructions::RRCA()
{
    CBRRCA();
    return 0;
}
int Instructions::RRA()
{
    CBRRA();
    return 0;
}

int Instructions::CBRLCA()
{
    CBRLCr(registers->a);
    return 0;
}
int Instructions::CBRLCB()
{
    CBRLCr(registers->b);
    return 0;
}
int Instructions::CBRLCC()
{
    CBRLCr(registers->c);
    return 0;
}
int Instructions::CBRLCD()
{
    CBRLCr(registers->d);
    return 0;
}
int Instructions::CBRLCE()
{
    CBRLCr(registers->e);
    return 0;
}
int Instructions::CBRLCH()
{
    CBRLCr(registers->h);
    return 0;
}
int Instructions::CBRLCL()
{
    CBRLCr(registers->l);
    return 0;
}
int Instructions::CBRLCHLm()
{
    uint8_t value = mmu->ReadByte(registers->hl);
    return 0;
    CBRLCr(value);
    return 0;
    return 0;
}

int Instructions::CBRLA()
{
    CBRLr(registers->a);
    return 0;
}
int Instructions::CBRLB()
{
    CBRLr(registers->b);
    return 0;
}
int Instructions::CBRLC()
{
    CBRLr(registers->c);
    return 0;
}
int Instructions::CBRLD()
{
    CBRLr(registers->d);
    return 0;
}
int Instructions::CBRLE()
{
    CBRLr(registers->e);
    return 0;
}
int Instructions::CBRLH()
{
    CBRLr(registers->h);
    return 0;
}
int Instructions::CBRLL()
{
    CBRLr(registers->l);
    return 0;
}
int Instructions::CBRLHLm()
{
    uint8_t value = mmu->ReadByte(registers->hl);
    return 0;
    CBRLr(value);
    return 0;
    return 0;
}

int Instructions::CBRRCA()
{
    CBRRCr(registers->a);
    return 0;
}
int Instructions::CBRRCB()
{
    CBRRCr(registers->b);
    return 0;
}
int Instructions::CBRRCC()
{
    CBRRCr(registers->c);
    return 0;
}
int Instructions::CBRRCD()
{
    CBRRCr(registers->d);
    return 0;
}
int Instructions::CBRRCE()
{
    CBRRCr(registers->e);
    return 0;
}
int Instructions::CBRRCH()
{
    CBRRCr(registers->h);
    return 0;
}
int Instructions::CBRRCL()
{
    CBRRCr(registers->l);
    return 0;
}
int Instructions::CBRRCHLm()
{
    uint8_t value = mmu->ReadByte(registers->hl);
    return 0;
    CBRRCr(value);
    return 0;
    return 0;
}

int Instructions::CBRRA()
{
    CBRRr(registers->a);
    return 0;
}
int Instructions::CBRRB()
{
    CBRRr(registers->b);
    return 0;
}
int Instructions::CBRRC()
{
    CBRRr(registers->c);
    return 0;
}
int Instructions::CBRRD()
{
    CBRRr(registers->d);
    return 0;
}
int Instructions::CBRRE()
{
    CBRRr(registers->e);
    return 0;
}
int Instructions::CBRRH()
{
    CBRRr(registers->h);
    return 0;
}
int Instructions::CBRRL()
{
    CBRRr(registers->l);
    return 0;
}
int Instructions::CBRRHLm()
{
    uint8_t value = mmu->ReadByte(registers->hl);
    return 0;
    CBRRr(value);
    return 0;
    return 0;
}

int Instructions::CBSLAA()
{
    CBSLAr(registers->a);
    return 0;
}
int Instructions::CBSLAB()
{
    CBSLAr(registers->b);
    return 0;
}
int Instructions::CBSLAC()
{
    CBSLAr(registers->c);
    return 0;
}
int Instructions::CBSLAD()
{
    CBSLAr(registers->d);
    return 0;
}
int Instructions::CBSLAE()
{
    CBSLAr(registers->e);
    return 0;
}
int Instructions::CBSLAH()
{
    CBSLAr(registers->h);
    return 0;
}
int Instructions::CBSLAL()
{
    CBSLAr(registers->l);
    return 0;
}
int Instructions::CBSLAHLm()
{
    uint8_t value = mmu->ReadByte(registers->hl);
    return 0;
    CBSLAr(value);
    return 0;
    return 0;
}

int Instructions::CBSRAA()
{
    CBSRAr(registers->a);
    return 0;
}
int Instructions::CBSRAB()
{
    CBSRAr(registers->b);
    return 0;
}
int Instructions::CBSRAC()
{
    CBSRAr(registers->c);
    return 0;
}
int Instructions::CBSRAD()
{
    CBSRAr(registers->d);
    return 0;
}
int Instructions::CBSRAE()
{
    CBSRAr(registers->e);
    return 0;
}
int Instructions::CBSRAH()
{
    CBSRAr(registers->h);
    return 0;
}
int Instructions::CBSRAL()
{
    CBSRAr(registers->l);
    return 0;
}
int Instructions::CBSRAHLm()
{
    uint8_t value = mmu->ReadByte(registers->hl);
    return 0;
    CBSRAr(value);
    return 0;
    return 0;
}

int Instructions::CBSRLA()
{
    CBSRLr(registers->a);
    return 0;
}
int Instructions::CBSRLB()
{
    CBSRLr(registers->b);
    return 0;
}
int Instructions::CBSRLC()
{
    CBSRLr(registers->c);
    return 0;
}
int Instructions::CBSRLD()
{
    CBSRLr(registers->d);
    return 0;
}
int Instructions::CBSRLE()
{
    CBSRLr(registers->e);
    return 0;
}
int Instructions::CBSRLH()
{
    CBSRLr(registers->h);
    return 0;
}
int Instructions::CBSRLL()
{
    CBSRLr(registers->l);
    return 0;
}
int Instructions::CBSRLHLm()
{
    uint8_t value = mmu->ReadByte(registers->hl);
    CBSRLr(value);
    return 0;
}

//****************//
//***** Jumps ****//
//****************//

int Instructions::JPnn()
{
    registers->pc = LoadImmediate16();
    return 0;
}
int Instructions::JPNZnn()
{
    if (!registers->GetFlag(Flags::Z)) registers->pc = LoadImmediate16();
    return 0;
}
int Instructions::JPZnn()
{
    if (registers->GetFlag(Flags::Z)) registers->pc = LoadImmediate16();
    return 0;
}
int Instructions::JPNCnn()
{
    if (!registers->GetFlag(Flags::C)) registers->pc = LoadImmediate16();
    return 0;
}
int Instructions::JPCnn()
{
    if (registers->GetFlag(Flags::C)) registers->pc = LoadImmediate16();
    return 0;
}
int Instructions::JPHLm()
{
    registers->pc = registers->hl;
    return 0;
}

int Instructions::JRn()
{
    registers->pc += LoadImmediate8();
    return 0;
}
int Instructions::JRNZn()
{
    if (!registers->GetFlag(Flags::Z)) registers->pc += (int8_t)LoadImmediate8();
    else registers->pc++;
    return 0;
}
int Instructions::JRZn()
{
    if (registers->GetFlag(Flags::Z)) registers->pc += (int8_t)LoadImmediate8();
    else registers->pc++;
    return 0;
}
int Instructions::JRNCn()
{
    if (!registers->GetFlag(Flags::C)) registers->pc += (int8_t)LoadImmediate8();
    else registers->pc++;
    return 0;
}
int Instructions::JRCn()
{
    if (registers->GetFlag(Flags::C)) registers->pc += (int8_t)LoadImmediate8();
    else registers->pc++;
    return 0;
}

//*****************//
//***** Calls *****//
//*****************//

//
int Instructions::CALLnn()
{
    uint16_t destAddr = LoadImmediate16();
    PushStack(registers->pc);
    registers->pc = destAddr;
    return 0;
}
int Instructions::CALLNZnn()
{
    if (!registers->GetFlag(Flags::Z))
        CALLnn();
    else
        registers->pc += 2;

    return 0;
}
int Instructions::CALLZnn()
{
    if (registers->GetFlag(Flags::Z)) CALLnn();
    else registers->pc += 2;
    return 0;
}
int Instructions::CALLNCnn()
{
    if (!registers->GetFlag(Flags::C)) CALLnn();
    else registers->pc += 2;
    return 0;
}
int Instructions::CALLCnn()
{
    if (registers->GetFlag(Flags::C)) CALLnn();
    else registers->pc += 2;
    return 0;
}

//********************//
//***** Restarts *****//
//********************//

int Instructions::RST00()
{
    RSTN(0);
    return 0;
}
int Instructions::RST08()
{
    RSTN(8);
    return 0;
}
int Instructions::RST10()
{
    RSTN(10);
    return 0;
}
int Instructions::RST18()
{
    RSTN(18);
    return 0;
}
int Instructions::RST20()
{
    RSTN(20);
    return 0;
}
int Instructions::RST28()
{
    RSTN(28);
    return 0;
}
int Instructions::RST30()
{
    RSTN(30);
    return 0;
}
int Instructions::RST38()
{
    RSTN(38);
    return 0;
}

//*******************//
//***** Returns *****//
//*******************//

int Instructions::RET()
{
    registers->pc = PopStack();
    return 0;
}

int Instructions::RETNZ()
{
    if (!registers->GetFlag(Flags::Z)) RET();
    return 0;
}
int Instructions::RETZ()
{
    if (registers->GetFlag(Flags::Z)) RET();
    return 0;
}
int Instructions::RETNC()
{
    if (!registers->GetFlag(Flags::C)) RET();
    return 0;
}
int Instructions::RETC()
{
    if (registers->GetFlag(Flags::C)) RET();
    return 0;
}

int Instructions::RETI()
{
    RET();
    // @todo Enable Interrupts
    return 0;
}

int Instructions::CBBIT0A()
{
    CBBITbr(0, registers->a);
    return 0;
}
int Instructions::CBBIT1A()
{
    CBBITbr(1, registers->a);
    return 0;
}
int Instructions::CBBIT2A()
{
    CBBITbr(2, registers->a);
    return 0;
}
int Instructions::CBBIT3A()
{
    CBBITbr(3, registers->a);
    return 0;
}
int Instructions::CBBIT4A()
{
    CBBITbr(4, registers->a);
    return 0;
}
int Instructions::CBBIT5A()
{
    CBBITbr(5, registers->a);
    return 0;
}
int Instructions::CBBIT6A()
{
    CBBITbr(6, registers->a);
    return 0;
}
int Instructions::CBBIT7A()
{
    CBBITbr(7, registers->a);
    return 0;
}
int Instructions::CBBIT0B()
{
    CBBITbr(0, registers->b);
    return 0;
}
int Instructions::CBBIT1B()
{
    CBBITbr(1, registers->b);
    return 0;
}
int Instructions::CBBIT2B()
{
    CBBITbr(2, registers->b);
    return 0;
}
int Instructions::CBBIT3B()
{
    CBBITbr(3, registers->b);
    return 0;
}
int Instructions::CBBIT4B()
{
    CBBITbr(4, registers->b);
    return 0;
}
int Instructions::CBBIT5B()
{
    CBBITbr(5, registers->b);
    return 0;
}
int Instructions::CBBIT6B()
{
    CBBITbr(6, registers->b);
    return 0;
}
int Instructions::CBBIT7B()
{
    CBBITbr(7, registers->b);
    return 0;
}
int Instructions::CBBIT0C()
{
    CBBITbr(0, registers->c);
    return 0;
}
int Instructions::CBBIT1C()
{
    CBBITbr(1, registers->c);
    return 0;
}
int Instructions::CBBIT2C()
{
    CBBITbr(2, registers->c);
    return 0;
}
int Instructions::CBBIT3C()
{
    CBBITbr(3, registers->c);
    return 0;
}
int Instructions::CBBIT4C()
{
    CBBITbr(4, registers->c);
    return 0;
}
int Instructions::CBBIT5C()
{
    CBBITbr(5, registers->c);
    return 0;
}
int Instructions::CBBIT6C()
{
    CBBITbr(6, registers->c);
    return 0;
}
int Instructions::CBBIT7C()
{
    CBBITbr(7, registers->c);
    return 0;
}
int Instructions::CBBIT0D()
{
    CBBITbr(0, registers->d);
    return 0;
}
int Instructions::CBBIT1D()
{
    CBBITbr(1, registers->d);
    return 0;
}
int Instructions::CBBIT2D()
{
    CBBITbr(2, registers->d);
    return 0;
}
int Instructions::CBBIT3D()
{
    CBBITbr(3, registers->d);
    return 0;
}
int Instructions::CBBIT4D()
{
    CBBITbr(4, registers->d);
    return 0;
}
int Instructions::CBBIT5D()
{
    CBBITbr(5, registers->d);
    return 0;
}
int Instructions::CBBIT6D()
{
    CBBITbr(6, registers->d);
    return 0;
}
int Instructions::CBBIT7D()
{
    CBBITbr(7, registers->d);
    return 0;
}
int Instructions::CBBIT0E()
{
    CBBITbr(0, registers->e);
    return 0;
}
int Instructions::CBBIT1E()
{
    CBBITbr(1, registers->e);
    return 0;
}
int Instructions::CBBIT2E()
{
    CBBITbr(2, registers->e);
    return 0;
}
int Instructions::CBBIT3E()
{
    CBBITbr(3, registers->e);
    return 0;
}
int Instructions::CBBIT4E()
{
    CBBITbr(4, registers->e);
    return 0;
}
int Instructions::CBBIT5E()
{
    CBBITbr(5, registers->e);
    return 0;
}
int Instructions::CBBIT6E()
{
    CBBITbr(6, registers->e);
    return 0;
}
int Instructions::CBBIT7E()
{
    CBBITbr(7, registers->e);
    return 0;
}
int Instructions::CBBIT0H()
{
    CBBITbr(0, registers->h);
    return 0;
}
int Instructions::CBBIT1H()
{
    CBBITbr(1, registers->h);
    return 0;
}
int Instructions::CBBIT2H()
{
    CBBITbr(2, registers->h);
    return 0;
}
int Instructions::CBBIT3H()
{
    CBBITbr(3, registers->h);
    return 0;
}
int Instructions::CBBIT4H()
{
    CBBITbr(4, registers->h);
    return 0;
}
int Instructions::CBBIT5H()
{
    CBBITbr(5, registers->h);
    return 0;
}
int Instructions::CBBIT6H()
{
    CBBITbr(6, registers->h);
    return 0;
}
int Instructions::CBBIT7H()
{
    CBBITbr(7, registers->h);
    return 0;
}
int Instructions::CBBIT0L()
{
    CBBITbr(0, registers->l);
    return 0;
}
int Instructions::CBBIT1L()
{
    CBBITbr(1, registers->l);
    return 0;
}
int Instructions::CBBIT2L()
{
    CBBITbr(2, registers->l);
    return 0;
}
int Instructions::CBBIT3L()
{
    CBBITbr(3, registers->l);
    return 0;
}
int Instructions::CBBIT4L()
{
    CBBITbr(4, registers->l);
    return 0;
}
int Instructions::CBBIT5L()
{
    CBBITbr(5, registers->l);
    return 0;
}
int Instructions::CBBIT6L()
{
    CBBITbr(6, registers->l);
    return 0;
}
int Instructions::CBBIT7L()
{
    CBBITbr(7, registers->l);
    return 0;
}
int Instructions::CBBIT0HLm()
{
    CBBITbr(0, mmu->GetMemoryRef(registers->hl));
    return 0;
}
int Instructions::CBBIT1HLm()
{
    CBBITbr(1, mmu->GetMemoryRef(registers->hl));
    return 0;
}
int Instructions::CBBIT2HLm()
{
    CBBITbr(2, mmu->GetMemoryRef(registers->hl));
    return 0;
}
int Instructions::CBBIT3HLm()
{
    CBBITbr(3, mmu->GetMemoryRef(registers->hl));
    return 0;
}
int Instructions::CBBIT4HLm()
{
    CBBITbr(4, mmu->GetMemoryRef(registers->hl));
    return 0;
}
int Instructions::CBBIT5HLm()
{
    CBBITbr(5, mmu->GetMemoryRef(registers->hl));
    return 0;
}
int Instructions::CBBIT6HLm()
{
    CBBITbr(6, mmu->GetMemoryRef(registers->hl));
    return 0;
}
int Instructions::CBBIT7HLm()
{
    CBBITbr(7, mmu->GetMemoryRef(registers->hl));
    return 0;
}

int Instructions::CBSET0A()
{
    CBSETbr(0, registers->a);
    return 0;
}
int Instructions::CBSET1A()
{
    CBSETbr(1, registers->a);
    return 0;
}
int Instructions::CBSET2A()
{
    CBSETbr(2, registers->a);
    return 0;
}
int Instructions::CBSET3A()
{
    CBSETbr(3, registers->a);
    return 0;
}
int Instructions::CBSET4A()
{
    CBSETbr(4, registers->a);
    return 0;
}
int Instructions::CBSET5A()
{
    CBSETbr(5, registers->a);
    return 0;
}
int Instructions::CBSET6A()
{
    CBSETbr(6, registers->a);
    return 0;
}
int Instructions::CBSET7A()
{
    CBSETbr(7, registers->a);
    return 0;
}
int Instructions::CBSET0B()
{
    CBSETbr(0, registers->b);
    return 0;
}
int Instructions::CBSET1B()
{
    CBSETbr(1, registers->b);
    return 0;
}
int Instructions::CBSET2B()
{
    CBSETbr(2, registers->b);
    return 0;
}
int Instructions::CBSET3B()
{
    CBSETbr(3, registers->b);
    return 0;
}
int Instructions::CBSET4B()
{
    CBSETbr(4, registers->b);
    return 0;
}
int Instructions::CBSET5B()
{
    CBSETbr(5, registers->b);
    return 0;
}
int Instructions::CBSET6B()
{
    CBSETbr(6, registers->b);
    return 0;
}
int Instructions::CBSET7B()
{
    CBSETbr(7, registers->b);
    return 0;
}
int Instructions::CBSET0C()
{
    CBSETbr(0, registers->c);
    return 0;
}
int Instructions::CBSET1C()
{
    CBSETbr(1, registers->c);
    return 0;
}
int Instructions::CBSET2C()
{
    CBSETbr(2, registers->c);
    return 0;
}
int Instructions::CBSET3C()
{
    CBSETbr(3, registers->c);
    return 0;
}
int Instructions::CBSET4C()
{
    CBSETbr(4, registers->c);
    return 0;
}
int Instructions::CBSET5C()
{
    CBSETbr(5, registers->c);
    return 0;
}
int Instructions::CBSET6C()
{
    CBSETbr(6, registers->c);
    return 0;
}
int Instructions::CBSET7C()
{
    CBSETbr(7, registers->c);
    return 0;
}
int Instructions::CBSET0D()
{
    CBSETbr(0, registers->d);
    return 0;
}
int Instructions::CBSET1D()
{
    CBSETbr(1, registers->d);
    return 0;
}
int Instructions::CBSET2D()
{
    CBSETbr(2, registers->d);
    return 0;
}
int Instructions::CBSET3D()
{
    CBSETbr(3, registers->d);
    return 0;
}
int Instructions::CBSET4D()
{
    CBSETbr(4, registers->d);
    return 0;
}
int Instructions::CBSET5D()
{
    CBSETbr(5, registers->d);
    return 0;
}
int Instructions::CBSET6D()
{
    CBSETbr(6, registers->d);
    return 0;
}
int Instructions::CBSET7D()
{
    CBSETbr(7, registers->d);
    return 0;
}
int Instructions::CBSET0E()
{
    CBSETbr(0, registers->e);
    return 0;
}
int Instructions::CBSET1E()
{
    CBSETbr(1, registers->e);
    return 0;
}
int Instructions::CBSET2E()
{
    CBSETbr(2, registers->e);
    return 0;
}
int Instructions::CBSET3E()
{
    CBSETbr(3, registers->e);
    return 0;
}
int Instructions::CBSET4E()
{
    CBSETbr(4, registers->e);
    return 0;
}
int Instructions::CBSET5E()
{
    CBSETbr(5, registers->e);
    return 0;
}
int Instructions::CBSET6E()
{
    CBSETbr(6, registers->e);
    return 0;
}
int Instructions::CBSET7E()
{
    CBSETbr(7, registers->e);
    return 0;
}
int Instructions::CBSET0H()
{
    CBSETbr(0, registers->h);
    return 0;
}
int Instructions::CBSET1H()
{
    CBSETbr(1, registers->h);
    return 0;
}
int Instructions::CBSET2H()
{
    CBSETbr(2, registers->h);
    return 0;
}
int Instructions::CBSET3H()
{
    CBSETbr(3, registers->h);
    return 0;
}
int Instructions::CBSET4H()
{
    CBSETbr(4, registers->h);
    return 0;
}
int Instructions::CBSET5H()
{
    CBSETbr(5, registers->h);
    return 0;
}
int Instructions::CBSET6H()
{
    CBSETbr(6, registers->h);
    return 0;
}
int Instructions::CBSET7H()
{
    CBSETbr(7, registers->h);
    return 0;
}
int Instructions::CBSET0L()
{
    CBSETbr(0, registers->l);
    return 0;
}
int Instructions::CBSET1L()
{
    CBSETbr(1, registers->l);
    return 0;
}
int Instructions::CBSET2L()
{
    CBSETbr(2, registers->l);
    return 0;
}
int Instructions::CBSET3L()
{
    CBSETbr(3, registers->l);
    return 0;
}
int Instructions::CBSET4L()
{
    CBSETbr(4, registers->l);
    return 0;
}
int Instructions::CBSET5L()
{
    CBSETbr(5, registers->l);
    return 0;
}
int Instructions::CBSET6L()
{
    CBSETbr(6, registers->l);
    return 0;
}
int Instructions::CBSET7L()
{
    CBSETbr(7, registers->l);
    return 0;
}
int Instructions::CBSET0HLm()
{
    CBSETbr(0, mmu->GetMemoryRef(registers->hl));
    return 0;
}
int Instructions::CBSET1HLm()
{
    CBSETbr(1, mmu->GetMemoryRef(registers->hl));
    return 0;
}
int Instructions::CBSET2HLm()
{
    CBSETbr(2, mmu->GetMemoryRef(registers->hl));
    return 0;
}
int Instructions::CBSET3HLm()
{
    CBSETbr(3, mmu->GetMemoryRef(registers->hl));
    return 0;
}
int Instructions::CBSET4HLm()
{
    CBSETbr(4, mmu->GetMemoryRef(registers->hl));
    return 0;
}
int Instructions::CBSET5HLm()
{
    CBSETbr(5, mmu->GetMemoryRef(registers->hl));
    return 0;
}
int Instructions::CBSET6HLm()
{
    CBSETbr(6, mmu->GetMemoryRef(registers->hl));
    return 0;
}
int Instructions::CBSET7HLm()
{
    CBSETbr(7, mmu->GetMemoryRef(registers->hl));
    return 0;
}

int Instructions::CBRES0A()
{
    CBRESbr(0, registers->a);
    return 0;
}
int Instructions::CBRES1A()
{
    CBRESbr(1, registers->a);
    return 0;
}
int Instructions::CBRES2A()
{
    CBRESbr(2, registers->a);
    return 0;
}
int Instructions::CBRES3A()
{
    CBRESbr(3, registers->a);
    return 0;
}
int Instructions::CBRES4A()
{
    CBRESbr(4, registers->a);
    return 0;
}
int Instructions::CBRES5A()
{
    CBRESbr(5, registers->a);
    return 0;
}
int Instructions::CBRES6A()
{
    CBRESbr(6, registers->a);
    return 0;
}
int Instructions::CBRES7A()
{
    CBRESbr(7, registers->a);
    return 0;
}
int Instructions::CBRES0B()
{
    CBRESbr(0, registers->b);
    return 0;
}
int Instructions::CBRES1B()
{
    CBRESbr(1, registers->b);
    return 0;
}
int Instructions::CBRES2B()
{
    CBRESbr(2, registers->b);
    return 0;
}
int Instructions::CBRES3B()
{
    CBRESbr(3, registers->b);
    return 0;
}
int Instructions::CBRES4B()
{
    CBRESbr(4, registers->b);
    return 0;
}
int Instructions::CBRES5B()
{
    CBRESbr(5, registers->b);
    return 0;
}
int Instructions::CBRES6B()
{
    CBRESbr(6, registers->b);
    return 0;
}
int Instructions::CBRES7B()
{
    CBRESbr(7, registers->b);
    return 0;
}
int Instructions::CBRES0C()
{
    CBRESbr(0, registers->c);
    return 0;
}
int Instructions::CBRES1C()
{
    CBRESbr(1, registers->c);
    return 0;
}
int Instructions::CBRES2C()
{
    CBRESbr(2, registers->c);
    return 0;
}
int Instructions::CBRES3C()
{
    CBRESbr(3, registers->c);
    return 0;
}
int Instructions::CBRES4C()
{
    CBRESbr(4, registers->c);
    return 0;
}
int Instructions::CBRES5C()
{
    CBRESbr(5, registers->c);
    return 0;
}
int Instructions::CBRES6C()
{
    CBRESbr(6, registers->c);
    return 0;
}
int Instructions::CBRES7C()
{
    CBRESbr(7, registers->c);
    return 0;
}
int Instructions::CBRES0D()
{
    CBRESbr(0, registers->d);
    return 0;
}
int Instructions::CBRES1D()
{
    CBRESbr(1, registers->d);
    return 0;
}
int Instructions::CBRES2D()
{
    CBRESbr(2, registers->d);
    return 0;
}
int Instructions::CBRES3D()
{
    CBRESbr(3, registers->d);
    return 0;
}
int Instructions::CBRES4D()
{
    CBRESbr(4, registers->d);
    return 0;
}
int Instructions::CBRES5D()
{
    CBRESbr(5, registers->d);
    return 0;
}
int Instructions::CBRES6D()
{
    CBRESbr(6, registers->d);
    return 0;
}
int Instructions::CBRES7D()
{
    CBRESbr(7, registers->d);
    return 0;
}
int Instructions::CBRES0E()
{
    CBRESbr(0, registers->e);
    return 0;
}
int Instructions::CBRES1E()
{
    CBRESbr(1, registers->e);
    return 0;
}
int Instructions::CBRES2E()
{
    CBRESbr(2, registers->e);
    return 0;
}
int Instructions::CBRES3E()
{
    CBRESbr(3, registers->e);
    return 0;
}
int Instructions::CBRES4E()
{
    CBRESbr(4, registers->e);
    return 0;
}
int Instructions::CBRES5E()
{
    CBRESbr(5, registers->e);
    return 0;
}
int Instructions::CBRES6E()
{
    CBRESbr(6, registers->e);
    return 0;
}
int Instructions::CBRES7E()
{
    CBRESbr(7, registers->e);
    return 0;
}
int Instructions::CBRES0H()
{
    CBRESbr(0, registers->h);
    return 0;
}
int Instructions::CBRES1H()
{
    CBRESbr(1, registers->h);
    return 0;
}
int Instructions::CBRES2H()
{
    CBRESbr(2, registers->h);
    return 0;
}
int Instructions::CBRES3H()
{
    CBRESbr(3, registers->h);
    return 0;
}
int Instructions::CBRES4H()
{
    CBRESbr(4, registers->h);
    return 0;
}
int Instructions::CBRES5H()
{
    CBRESbr(5, registers->h);
    return 0;
}
int Instructions::CBRES6H()
{
    CBRESbr(6, registers->h);
    return 0;
}
int Instructions::CBRES7H()
{
    CBRESbr(7, registers->h);
    return 0;
}
int Instructions::CBRES0L()
{
    CBRESbr(0, registers->l);
    return 0;
}
int Instructions::CBRES1L()
{
    CBRESbr(1, registers->l);
    return 0;
}
int Instructions::CBRES2L()
{
    CBRESbr(2, registers->l);
    return 0;
}
int Instructions::CBRES3L()
{
    CBRESbr(3, registers->l);
    return 0;
}
int Instructions::CBRES4L()
{
    CBRESbr(4, registers->l);
    return 0;
}
int Instructions::CBRES5L()
{
    CBRESbr(5, registers->l);
    return 0;
}
int Instructions::CBRES6L()
{
    CBRESbr(6, registers->l);
    return 0;
}
int Instructions::CBRES7L()
{
    CBRESbr(7, registers->l);
    return 0;
}
int Instructions::CBRES0HLm()
{
    CBRESbr(0, mmu->GetMemoryRef(registers->hl));
    return 0;
}
int Instructions::CBRES1HLm()
{
    CBRESbr(1, mmu->GetMemoryRef(registers->hl));
    return 0;
}
int Instructions::CBRES2HLm()
{
    CBRESbr(2, mmu->GetMemoryRef(registers->hl));
    return 0;
}
int Instructions::CBRES3HLm()
{
    CBRESbr(3, mmu->GetMemoryRef(registers->hl));
    return 0;
}
int Instructions::CBRES4HLm()
{
    CBRESbr(4, mmu->GetMemoryRef(registers->hl));
    return 0;
}
int Instructions::CBRES5HLm()
{
    CBRESbr(5, mmu->GetMemoryRef(registers->hl));
    return 0;
}
int Instructions::CBRES6HLm()
{
    CBRESbr(6, mmu->GetMemoryRef(registers->hl));
    return 0;
}
int Instructions::CBRES7HLm()
{
    CBRESbr(7, mmu->GetMemoryRef(registers->hl));
    return 0;
}

int Instructions::CBSWAPA()
{
    CBSWAPn(registers->a);
    return 0;
}
int Instructions::CBSWAPB()
{
    CBSWAPn(registers->b);
    return 0;
}
int Instructions::CBSWAPC()
{
    CBSWAPn(registers->c);
    return 0;
}
int Instructions::CBSWAPD()
{
    CBSWAPn(registers->d);
    return 0;
}
int Instructions::CBSWAPE()
{
    CBSWAPn(registers->e);
    return 0;
}
int Instructions::CBSWAPH()
{
    CBSWAPn(registers->h);
    return 0;
}
int Instructions::CBSWAPL()
{
    CBSWAPn(registers->l);
    return 0;
}
int Instructions::CBSWAPHLm()
{
    CBSWAPn(mmu->GetMemoryRef(registers->hl));
    return 0;
}


// Calls a CB instruction
int Instructions::CBInst()
{
    uint8_t opcode = LoadImmediate8();
    (this->*CBOpcodes[opcode])();
    return 0;
}
