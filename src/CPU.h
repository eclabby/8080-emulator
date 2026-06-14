#ifndef CPU_H
#define CPU_H

#include <vector>
#include <string>

#include "common.h"


// rh   rl      register pair
// --------------------------
// A    F   =   PSW
// B    C   =   B
// D    E   =   D
// H    L   =   H
struct CPU_Reg_Array
{
    u16 pc;
    u16 sp;
    u8 a;
    u8 f;
    u8 b;
    u8 c;
    u8 d;
    u8 e;
    u8 h;
    u8 l;
};

struct CPU_Context
{
    bool running;
    struct CPU_Reg_Array regs;
    u8 instr_reg; // Instruction Register (current opcode)
    u16 data;
    u16 addr;
    u8 INTE;
    u8 halted;
};



class CPU
{
public:
    CPU();


    struct CPU_Context ctx;


    void cpu_init();
    void cpu_reset();
    void cpu_step();


private:
    void fetch();
    void execute();


    enum FLAGS
    {
        FLAG_S = (1 << 7),    // sign
        FLAG_Z = (1 << 6),    // zero
        FLAG_AC = (1 << 4),   // auxilliary carry
        FLAG_P = (1 << 2),    // parity
        FLAG_CY = (1 << 0)    // carry
    };

    enum Register
    {
        REG_B = 0,
        REG_C = 1,
        REG_D = 2,
        REG_E = 3,
        REG_H = 4,
        REG_L = 5,
        REG_HL = 6,
        REG_A = 7,
        REG_BC,
        REG_DE,
        REG_SP,
        REG_PSW,
        REG_NONE
    };


    u8 mem_read(u16 addr);
    void mem_write(u16 addr, u8 data);

    u8 get_flag(FLAGS flag);
    void set_flag(FLAGS flag);
    void reset_flag(FLAGS flag);
    u8 parity(u8 val);
    void update_flags_SZP(u8 result); // sign, zero, and parity

    u16 read_register(enum Register reg);
    void set_register(enum Register reg, u16 data);


    struct Instruction
    {
        std::string mnemonic;
        void (CPU::*instr)(void) = nullptr; // Instruction
        void (CPU::*mode)(void) = nullptr; // Addressing Mode
        u8 cycles = 0;
        enum Register reg = REG_NONE;
    };

    std::vector<Instruction> instructions;

private:
    // Addressing Modes
    void AM_DIR();
    void AM_REG();
    void AM_IND();
    void AM_IMM_D8();
    void AM_IMM_D16();
    void AM_NONE();

    // Instruction Set
    void MOV_r_r();
    void MOV_r_M();
    void MOV_M_r();
    void MVI_r();
    void MVI_M();
    void LXI();
    void LDA();
    void STA();
    void LHLD();
    void SHLD();
    void LDAX();
    void STAX();
    void XCHG();

    void ADD();
    void SUB();
    void INR();
    void DCR();
    void INX();
    void DCX();
    void DAD();
    void DAA();

    void ANA_r();
    void ANA_M();
    void ANI();
    void XRA_r();
    void XRA_M();
    void XRI();
    void ORA_r();
    void ORA_M();
    void ORI();
    void CMP_r();
    void CMP_M();
    void CPI();
    void RLC();
    void RRC();
    void RAL();
    void RAR();
    void CMA();
    void CMC();
    void STC();

    void JMP();
    void JMP_cond();
    void CALL();
    void CALL_cond();
    void RET();
    void RET_cond();
    void RST();
    void PCHL();

    void PUSH();
    void POP();
    void XTHL();
    void SPHL();
    void IN();
    void OUT();
    void EI();
    void DI();
    void HLT();
    void NOP();

};

#endif
