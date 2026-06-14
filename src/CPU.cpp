// 8080 8-bit CPU Emulator

#include "CPU.h"
#include "Bus.h"

using namespace std;



CPU::CPU()
{
    instructions =
    {
        {"NOP", &CPU::NOP, &CPU::AM_NONE, 4},                         // 0x00
        {"LXI B,d16", &CPU::LXI, &CPU::AM_IMM_D16, 10, REG_BC},       // 0x01
        {"STAX B", &CPU::STAX, &CPU::AM_IND, 7, REG_BC},              // 0x02
        {"INX B", &CPU::INX, &CPU::AM_REG, 5, REG_BC},                // 0x03
        {"INR B", &CPU::INR, &CPU::AM_REG, 5, REG_B},                 // 0x04
        {"DCR B", &CPU::DCR, &CPU::AM_REG, 5, REG_B},                 // 0x05
        {"MVI B,d8", &CPU::MVI_r, &CPU::AM_REG, 7, REG_B},            // 0x06
        {"RLC", &CPU::RLC, &CPU::AM_NONE, 4},                         // 0x07
        {"*NOP", &CPU::NOP, &CPU::AM_NONE, 4},                        // 0x08
        {"DAD B", &CPU::DAD, &CPU::AM_REG, 10, REG_BC},               // 0x09
        {"LDAX B", &CPU::LDAX, &CPU::AM_IND, 7, REG_BC},              // 0x0A
        {"DCX B", &CPU::DCX, &CPU::AM_REG, 5, REG_BC},
        {"INR C", &CPU::INR, &CPU::AM_REG, 5, REG_C},
        {"DCR C", &CPU::DCR, &CPU::AM_REG, 5, REG_C},
        {"MVI C,d8", &CPU::MVI_r, &CPU::AM_REG, 7, REG_C},
        {"RRC", &CPU::RRC, &CPU::AM_NONE, 4},

        {"*NOP", &CPU::NOP, &CPU::AM_NONE, 4},
        {"LXI D,d16", &CPU::LXI, &CPU::AM_IMM_D16, 10, REG_DE},
        {"STAX D", &CPU::STAX, &CPU::AM_IND, 7, REG_DE},
        {"INX D", &CPU::INX, &CPU::AM_REG, 5, REG_DE},
        {"INR D", &CPU::INR, &CPU::AM_REG, 5, REG_D},
        {"DCR D", &CPU::DCR, &CPU::AM_REG, 5, REG_D},
        {"MVI D,d8", &CPU::MVI_r, &CPU::AM_REG, 7, REG_D},
        {"RAL", &CPU::RAL, &CPU::AM_NONE, 4},
        {"*NOP", &CPU::NOP, &CPU::AM_NONE, 4},
        {"DAD D", &CPU::DAD, &CPU::AM_REG, 10, REG_DE},
        {"LDAX D", &CPU::LDAX, &CPU::AM_IND, 7, REG_DE},
        {"DCX D", &CPU::DCX, &CPU::AM_REG, 5, REG_DE},
        {"INR E", &CPU::INR, &CPU::AM_REG, 5, REG_E},
        {"DCR E", &CPU::DCR, &CPU::AM_REG, 5, REG_E},
        {"MVI E,d8", &CPU::MVI_r, &CPU::AM_REG, 7, REG_E},
        {"RAR", &CPU::RAR, &CPU::AM_NONE, 4},

        {"*NOP", &CPU::NOP, &CPU::AM_NONE, 4},
        {"LXI H,d16", &CPU::LXI, &CPU::AM_IMM_D16, 10, REG_HL},
        {"SHLD", &CPU::SHLD, &CPU::AM_DIR, 16},
        {"INX H", &CPU::INX, &CPU::AM_REG, 5, REG_HL},
        {"INR H", &CPU::INR, &CPU::AM_REG, 5, REG_H},
        {"DCR H", &CPU::DCR, &CPU::AM_REG, 5, REG_H},
        {"MVI H,d8", &CPU::MVI_r, &CPU::AM_REG, 7, REG_H},
        {"DAA", &CPU::DAA, &CPU::AM_NONE, 4},
        {"*NOP", &CPU::NOP, &CPU::AM_NONE, 4},
        {"DAD H", &CPU::DAD, &CPU::AM_REG, 10, REG_HL},
        {"LHLD", &CPU::LHLD, &CPU::AM_IMM_D16, 16},
        {"DCX H", &CPU::DCX, &CPU::AM_REG, 5, REG_HL},
        {"INR L", &CPU::INR, &CPU::AM_REG, 5, REG_L},
        {"DCR L", &CPU::DCR, &CPU::AM_REG, 5, REG_L},
        {"MVI L,d8", &CPU::MVI_r, &CPU::AM_REG, 7, REG_L},
        {"CMA", &CPU::CMA, &CPU::AM_NONE, 4},

        {"*NOP", &CPU::NOP, &CPU::AM_NONE, 4},
        {"LXI SP,d16", &CPU::LXI, &CPU::AM_IMM_D16, 10, REG_SP},
        {"STA", &CPU::STA, &CPU::AM_DIR, 13},                         // 0x32
        {"INX SP", &CPU::INX, &CPU::AM_REG, 5, REG_SP},
        {"INR M", &CPU::INR, &CPU::AM_IND, 10, REG_HL},
        {"DCR M", &CPU::DCR, &CPU::AM_IND, 10, REG_HL},
        {"MVI M,d8", &CPU::MVI_M, &CPU::AM_IMM_D8, 10, REG_HL},
        {"STC", &CPU::STC, &CPU::AM_NONE, 4},
        {"*NOP", &CPU::NOP, &CPU::AM_NONE, 4},
        {"DAD SP", &CPU::DAD, &CPU::AM_REG, 10, REG_SP},
        {"LDA", &CPU::LDA, &CPU::AM_DIR, 13},
        {"DCX SP", &CPU::DCX, &CPU::AM_REG, 5, REG_SP},
        {"INR A", &CPU::INR, &CPU::AM_REG, 5, REG_A},
        {"DCR A", &CPU::DCR, &CPU::AM_REG, 5, REG_A},
        {"MVI A,d8", &CPU::MVI_r, &CPU::AM_REG, 7, REG_A},
        {"CMC", &CPU::CMA, &CPU::AM_NONE, 4},

        {"MOV B,B", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_B},
        {"MOV B,C", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_C},
        {"MOV B,D", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_D},
        {"MOV B,E", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_E},
        {"MOV B,H", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_H},
        {"MOV B,L", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_L},
        {"MOV B,M", &CPU::MOV_r_M, &CPU::AM_IND, 7, REG_HL},
        {"MOV B,A", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_A},
        {"MOV C,B", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_B},
        {"MOV C,C", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_C},
        {"MOV C,D", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_D},
        {"MOV C,E", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_E},
        {"MOV C,H", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_H},
        {"MOV C,L", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_L},
        {"MOV C,M", &CPU::MOV_r_M, &CPU::AM_IND, 7, REG_HL},
        {"MOV C,A", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_A},

        {"MOV D,B", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_B},
        {"MOV D,C", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_C},
        {"MOV D,D", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_D},
        {"MOV D,E", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_E},
        {"MOV D,H", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_H},
        {"MOV D,L", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_L},
        {"MOV D,M", &CPU::MOV_r_M, &CPU::AM_IND, 7, REG_HL},
        {"MOV D,A", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_A},
        {"MOV E,B", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_B},
        {"MOV E,C", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_C},
        {"MOV E,D", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_D},
        {"MOV E,E", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_E},
        {"MOV E,H", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_H},
        {"MOV E,L", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_L},
        {"MOV E,M", &CPU::MOV_r_M, &CPU::AM_IND, 7, REG_HL},
        {"MOV E,A", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_A},

        {"MOV H,B", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_B},
        {"MOV H,C", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_C},
        {"MOV H,D", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_D},
        {"MOV H,E", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_E},
        {"MOV H,H", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_H},
        {"MOV H,L", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_L},
        {"MOV H,M", &CPU::MOV_r_M, &CPU::AM_IND, 7, REG_HL},
        {"MOV H,A", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_A},
        {"MOV L,B", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_B},
        {"MOV L,C", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_C},
        {"MOV L,D", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_D},
        {"MOV L,E", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_E},
        {"MOV L,H", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_H},
        {"MOV L,L", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_L},
        {"MOV L,M", &CPU::MOV_r_M, &CPU::AM_IND, 7, REG_HL},
        {"MOV L,A", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_A},

        {"MOV M,B", &CPU::MOV_M_r, &CPU::AM_IND, 7, REG_B},
        {"MOV M,C", &CPU::MOV_M_r, &CPU::AM_IND, 7, REG_C},
        {"MOV M,D", &CPU::MOV_M_r, &CPU::AM_IND, 7, REG_D},
        {"MOV M,E", &CPU::MOV_M_r, &CPU::AM_IND, 7, REG_E},
        {"MOV M,H", &CPU::MOV_M_r, &CPU::AM_IND, 7, REG_H},
        {"MOV M,L", &CPU::MOV_M_r, &CPU::AM_IND, 7, REG_L},
        {"HLT", &CPU::HLT, &CPU::AM_NONE, 7},
        {"MOV M,A", &CPU::MOV_M_r, &CPU::AM_IND, 7, REG_A},
        {"MOV A,B", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_B},
        {"MOV A,C", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_C},
        {"MOV A,D", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_D},
        {"MOV A,E", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_E},
        {"MOV A,H", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_H},
        {"MOV A,L", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_L},
        {"MOV A,M", &CPU::MOV_r_M, &CPU::AM_IND, 7, REG_HL},
        {"MOV A,A", &CPU::MOV_r_r, &CPU::AM_REG, 5, REG_A},

        {"ADD B", &CPU::ADD, &CPU::AM_REG, 4, REG_B},
        {"ADD C", &CPU::ADD, &CPU::AM_REG, 4, REG_C},
        {"ADD D", &CPU::ADD, &CPU::AM_REG, 4, REG_D},
        {"ADD E", &CPU::ADD, &CPU::AM_REG, 4, REG_E},
        {"ADD H", &CPU::ADD, &CPU::AM_REG, 4, REG_H},
        {"ADD L", &CPU::ADD, &CPU::AM_REG, 4, REG_L},
        {"ADD M", &CPU::ADD, &CPU::AM_IND, 7, REG_HL},
        {"ADD A", &CPU::ADD, &CPU::AM_REG, 4, REG_B},
        {"ADC B", &CPU::ADD, &CPU::AM_REG, 4, REG_B},
        {"ADC C", &CPU::ADD, &CPU::AM_REG, 4, REG_C},
        {"ADC D", &CPU::ADD, &CPU::AM_REG, 4, REG_D},
        {"ADC E", &CPU::ADD, &CPU::AM_REG, 4, REG_E},
        {"ADC H", &CPU::ADD, &CPU::AM_REG, 4, REG_H},
        {"ADC L", &CPU::ADD, &CPU::AM_REG, 4, REG_L},
        {"ADC M", &CPU::ADD, &CPU::AM_IND, 7, REG_HL},
        {"ADC A", &CPU::ADD, &CPU::AM_REG, 4, REG_A},

        {"SUB B", &CPU::SUB, &CPU::AM_REG, 4, REG_B},
        {"SUB C", &CPU::SUB, &CPU::AM_REG, 4, REG_C},
        {"SUB D", &CPU::SUB, &CPU::AM_REG, 4, REG_D},
        {"SUB E", &CPU::SUB, &CPU::AM_REG, 4, REG_E},
        {"SUB H", &CPU::SUB, &CPU::AM_REG, 4, REG_H},
        {"SUB L", &CPU::SUB, &CPU::AM_REG, 4, REG_L},
        {"SUB M", &CPU::SUB, &CPU::AM_IND, 7, REG_HL},
        {"SUB A", &CPU::SUB, &CPU::AM_REG, 4, REG_A},
        {"SBB B", &CPU::SUB, &CPU::AM_REG, 4, REG_B},
        {"SBB C", &CPU::SUB, &CPU::AM_REG, 4, REG_C},
        {"SBB D", &CPU::SUB, &CPU::AM_REG, 4, REG_D},
        {"SBB E", &CPU::SUB, &CPU::AM_REG, 4, REG_E},
        {"SBB H", &CPU::SUB, &CPU::AM_REG, 4, REG_H},
        {"SBB L", &CPU::SUB, &CPU::AM_REG, 4, REG_L},
        {"SBB M", &CPU::SUB, &CPU::AM_IND, 7, REG_HL},
        {"SBB A", &CPU::SUB, &CPU::AM_REG, 4, REG_A},

        {"ANA B", &CPU::ANA_r, &CPU::AM_REG, 4, REG_B},
        {"ANA C", &CPU::ANA_r, &CPU::AM_REG, 4, REG_C},
        {"ANA D", &CPU::ANA_r, &CPU::AM_REG, 4, REG_D},
        {"ANA E", &CPU::ANA_r, &CPU::AM_REG, 4, REG_E},
        {"ANA H", &CPU::ANA_r, &CPU::AM_REG, 4, REG_H},
        {"ANA L", &CPU::ANA_r, &CPU::AM_REG, 4, REG_L},
        {"ANA M", &CPU::ANA_M, &CPU::AM_IND, 7, REG_HL},
        {"ANA A", &CPU::ANA_r, &CPU::AM_REG, 4, REG_A},
        {"XRA B", &CPU::XRA_r, &CPU::AM_REG, 4, REG_B},
        {"XRA C", &CPU::XRA_r, &CPU::AM_REG, 4, REG_C},
        {"XRA D", &CPU::XRA_r, &CPU::AM_REG, 4, REG_D},
        {"XRA E", &CPU::XRA_r, &CPU::AM_REG, 4, REG_E},
        {"XRA H", &CPU::XRA_r, &CPU::AM_REG, 4, REG_H},
        {"XRA L", &CPU::XRA_r, &CPU::AM_REG, 4, REG_L},
        {"XRA M", &CPU::XRA_M, &CPU::AM_IND, 7, REG_HL},
        {"XRA A", &CPU::XRA_r, &CPU::AM_REG, 4, REG_A},

        {"ORA B", &CPU::ORA_r, &CPU::AM_REG, 4, REG_B},
        {"ORA C", &CPU::ORA_r, &CPU::AM_REG, 4, REG_C},
        {"ORA D", &CPU::ORA_r, &CPU::AM_REG, 4, REG_D},
        {"ORA E", &CPU::ORA_r, &CPU::AM_REG, 4, REG_E},
        {"ORA H", &CPU::ORA_r, &CPU::AM_REG, 4, REG_H},
        {"ORA L", &CPU::ORA_r, &CPU::AM_REG, 4, REG_L},
        {"ORA M", &CPU::ORA_M, &CPU::AM_IND, 7, REG_HL},
        {"ORA A", &CPU::ORA_r, &CPU::AM_REG, 4, REG_A},
        {"CMP B", &CPU::CMP_r, &CPU::AM_REG, 4, REG_B},
        {"CMP C", &CPU::CMP_r, &CPU::AM_REG, 4, REG_C},
        {"CMP D", &CPU::CMP_r, &CPU::AM_REG, 4, REG_D},
        {"CMP E", &CPU::CMP_r, &CPU::AM_REG, 4, REG_E},
        {"CMP H", &CPU::CMP_r, &CPU::AM_REG, 4, REG_H},
        {"CMP L", &CPU::CMP_r, &CPU::AM_REG, 4, REG_L},
        {"CMP M", &CPU::CMP_M, &CPU::AM_IND, 7, REG_HL},
        {"CMP A", &CPU::CMP_r, &CPU::AM_REG, 4, REG_A},

        // TODO some instructions past here may require additional cycles

        {"RNZ", &CPU::RET_cond, &CPU::AM_IND, 5, REG_SP},
        {"POP B", &CPU::POP, &CPU::AM_IND, 10, REG_SP},
        {"JNZ", &CPU::JMP_cond, &CPU::AM_IMM_D16, 10},
        {"JMP", &CPU::JMP, &CPU::AM_IMM_D16, 10},
        {"CNZ", &CPU::CALL_cond, &CPU::AM_IMM_D16, 11},
        {"PUSH B", &CPU::PUSH, &CPU::AM_IND, 11, REG_BC},
        {"ADI", &CPU::ADD, &CPU::AM_IMM_D8, 7},
        {"RST 0", &CPU::RST, &CPU::AM_IND, 11},
        {"RZ", &CPU::RET_cond, &CPU::AM_IND, 5, REG_SP},
        {"RET", &CPU::RET, &CPU::AM_IND, 10, REG_SP},
        {"JZ", &CPU::JMP_cond, &CPU::AM_IMM_D16, 10},
        {"*JMP", &CPU::JMP, &CPU::AM_IMM_D16, 10},
        {"CZ", &CPU::CALL_cond, &CPU::AM_IMM_D16, 11},
        {"CALL", &CPU::CALL, &CPU::AM_IMM_D16, 17},
        {"ACI", &CPU::ADD, &CPU::AM_IMM_D8, 7},
        {"RST 1", &CPU::RST, &CPU::AM_IND, 11},

        {"RNC", &CPU::RET_cond, &CPU::AM_IND, 5, REG_SP},
        {"POP D", &CPU::POP, &CPU::AM_IND, 10, REG_SP},
        {"JNC", &CPU::JMP_cond, &CPU::AM_IMM_D16, 10},
        {"OUT", &CPU::OUT, &CPU::AM_DIR, 10},
        {"CNC", &CPU::CALL_cond, &CPU::AM_IMM_D16, 11},
        {"PUSH D", &CPU::PUSH, &CPU::AM_IND, 11, REG_DE},
        {"SUI", &CPU::SUB, &CPU::AM_IMM_D8, 7},
        {"RST 2", &CPU::RST, &CPU::AM_IND, 11},
        {"RC", &CPU::RET_cond, &CPU::AM_IND, 5, REG_SP},
        {"*RET", &CPU::RET, &CPU::AM_IND, 10, REG_SP},
        {"JC", &CPU::JMP_cond, &CPU::AM_IMM_D16, 10},
        {"IN", &CPU::IN, &CPU::AM_DIR, 10},
        {"CC", &CPU::CALL_cond, &CPU::AM_IMM_D16, 11},
        {"*CALL", &CPU::CALL, &CPU::AM_IMM_D16, 17},
        {"SBI", &CPU::SUB, &CPU::AM_IMM_D8, 7},
        {"RST 3", &CPU::RST, &CPU::AM_IND, 11},

        {"RPO", &CPU::RET_cond, &CPU::AM_IND, 5, REG_SP},
        {"POP H", &CPU::POP, &CPU::AM_IND, 10, REG_SP},
        {"JPO", &CPU::JMP_cond, &CPU::AM_IMM_D16, 10},
        {"XTHL", &CPU::XTHL, &CPU::AM_IND, 18, REG_HL},
        {"CPO", &CPU::CALL_cond, &CPU::AM_IMM_D16, 11},
        {"PUSH H", &CPU::PUSH, &CPU::AM_IND, 11, REG_HL},
        {"ANI", &CPU::ANI, &CPU::AM_IMM_D8, 7},
        {"RST 4", &CPU::RST, &CPU::AM_IND, 11},
        {"RPE", &CPU::RET_cond, &CPU::AM_IND, 5, REG_SP},
        {"PCHL", &CPU::PCHL, &CPU::AM_REG, 5, REG_HL},
        {"JPE", &CPU::JMP_cond, &CPU::AM_IMM_D16, 10},
        {"XCHG", &CPU::XCHG, &CPU::AM_REG, 4, REG_DE},
        {"CPE", &CPU::CALL_cond, &CPU::AM_IMM_D16, 11},
        {"*CALL", &CPU::CALL, &CPU::AM_IMM_D16, 17},
        {"XRI", &CPU::XRI, &CPU::AM_IMM_D8, 7},
        {"RST 5", &CPU::RST, &CPU::AM_IND, 11},

        {"RP", &CPU::RET_cond, &CPU::AM_IND, 5, REG_SP},
        {"POP PSW", &CPU::POP, &CPU::AM_IND, 10, REG_PSW},
        {"JP", &CPU::JMP_cond, &CPU::AM_IMM_D16, 10},
        {"DI", &CPU::DI, &CPU::AM_NONE, 4},
        {"CP", &CPU::CALL_cond, &CPU::AM_IMM_D16, 11},
        {"PUSH PSW", &CPU::PUSH, &CPU::AM_IND, 11, REG_PSW},
        {"ORI", &CPU::ORI, &CPU::AM_IMM_D8, 7},
        {"RST 6", &CPU::RST, &CPU::AM_IND, 11},
        {"RM", &CPU::RET_cond, &CPU::AM_IND, 5, REG_SP},
        {"SPHL", &CPU::SPHL, &CPU::AM_REG, 5, REG_HL},
        {"JM", &CPU::JMP_cond, &CPU::AM_IMM_D16, 10},
        {"EI", &CPU::EI, &CPU::AM_NONE, 4},
        {"CM", &CPU::CALL_cond, &CPU::AM_IMM_D16, 11},
        {"*CALL", &CPU::CALL, &CPU::AM_IMM_D16, 17},
        {"CPI", &CPU::CPI, &CPU::AM_IMM_D8, 7},
        {"RST 7", &CPU::RST, &CPU::AM_IND, 11}
    };
}

void CPU::cpu_init()
{
    ctx.running = true;

    ctx.regs.pc = 0x0000;
    ctx.regs.sp = 0x0000;
    ctx.regs.a = 0x00;
    ctx.regs.f = 0x02;
    ctx.regs.b = 0x00;
    ctx.regs.c = 0x00;
    ctx.regs.d = 0x00;
    ctx.regs.e = 0x00;
    ctx.regs.h = 0x00;
    ctx.regs.l = 0x00;

    ctx.instr_reg = 0;
    ctx.INTE = 0;
    ctx.halted = 0;
}

// TODO connect cpu to bus
u8 CPU::mem_read(u16 addr)
{
    // bus_read()
    return 0;
}

void CPU::mem_write(u16 addr, u8 data)
{
    // bus_write()
}


void CPU::fetch()
{
    ctx.instr_reg = mem_read(ctx.regs.pc++);
}

void CPU::execute()
{
    (this->*instructions[ctx.instr_reg].mode)();
    (this->*instructions[ctx.instr_reg].instr)();
}

void CPU::cpu_step()
{
    if (!ctx.halted)
    {
        fetch();
        execute();
    }
}


u8 CPU::get_flag(FLAGS flag)
{
    return ctx.regs.f & flag;
}

void CPU::set_flag(FLAGS flag)
{
    ctx.regs.f |= flag;
}

void CPU::reset_flag(FLAGS flag)
{
    ctx.regs.f &= ~(flag);
}

u8 CPU::parity(u8 val)
{
    u8 sum_bits = 0;
    for (int i = 0; i < 8; i++)
    {
        sum_bits += ((val >> i) & 1);
    }

    return (sum_bits % 2) == 0;
}

void CPU::update_flags_SZP(u8 result)
{
    // Sign
    (result >> 7) ? set_flag(FLAG_S) : reset_flag(FLAG_S);

    // Zero
    (result == 0) ? set_flag(FLAG_Z) : reset_flag(FLAG_Z);

    // Even Parity
    parity(result) ? set_flag(FLAG_P) : reset_flag(FLAG_P);
}


u16 CPU::read_register(enum Register reg)
{
    switch (reg)
    {
        case REG_A : return ctx.regs.a;
        case REG_B : return ctx.regs.b;
        case REG_C : return ctx.regs.c;
        case REG_D : return ctx.regs.d;
        case REG_E : return ctx.regs.e;
        case REG_H : return ctx.regs.h;
        case REG_L : return ctx.regs.l;

        case REG_PSW : return (ctx.regs.a << 8) | ctx.regs.f;
        case REG_BC  : return (ctx.regs.b << 8) | ctx.regs.c;
        case REG_DE  : return (ctx.regs.d << 8) | ctx.regs.e;
        case REG_HL  : return (ctx.regs.h << 8) | ctx.regs.l;

        case REG_SP  : return ctx.regs.sp;

        default : return 0;
    }
}

void CPU::set_register(enum Register reg, u16 data)
{
    switch (reg)
    {
        case REG_A : ctx.regs.a = data; break;
        case REG_B : ctx.regs.b = data; break;
        case REG_C : ctx.regs.c = data; break;
        case REG_D : ctx.regs.d = data; break;
        case REG_E : ctx.regs.e = data; break;
        case REG_H : ctx.regs.h = data; break;
        case REG_L : ctx.regs.l = data; break;

        case REG_PSW :
            ctx.regs.a = data >> 8;
            ctx.regs.f = data & 0xFF;
            break;
        case REG_BC :
            ctx.regs.b = data >> 8;
            ctx.regs.c = data & 0xFF;
            break;
        case REG_DE :
            ctx.regs.d = data >> 8;
            ctx.regs.e = data & 0xFF;
            break;
        case REG_HL :
            ctx.regs.h = data >> 8;
            ctx.regs.l = data & 0xFF;
        case REG_SP :
            ctx.regs.sp = data;
    }
}





////////////////////////////////////////////////////////
// ADDRESSING MODES                                   //
////////////////////////////////////////////////////////

// DIR : Direct
// REG : Register
// IND : Register Indirect
// IMM : Immediate

void CPU::AM_DIR()
{
    u8 lo = mem_read(ctx.regs.pc);
    u8 hi = mem_read(ctx.regs.pc + 1);
    ctx.regs.pc += 2;

    ctx.addr = (hi << 8) | lo;
}

void CPU::AM_REG()
{
    ctx.data = read_register(instructions[ctx.instr_reg].reg);
}

void CPU::AM_IND()
{
    ctx.addr = read_register(instructions[ctx.instr_reg].reg);
}

void CPU::AM_IMM_D8()
{
    ctx.data = mem_read(ctx.regs.pc++);
}

void CPU::AM_IMM_D16()
{
    u8 lo = mem_read(ctx.regs.pc);
    u8 hi = mem_read(ctx.regs.pc + 1);
    ctx.regs.pc += 2;

    ctx.data = (hi << 8) | lo;
}

void CPU::AM_NONE()
{
}




////////////////////////////////////////////////////////
// INSTRUCTION SET                                    //
////////////////////////////////////////////////////////

// Data Transfer Group

void CPU::MOV_r_r()
{
    int dst = (ctx.instr_reg >> 3) & 7;

    set_register((Register)dst, ctx.data);
}

void CPU::MOV_r_M()
{
    int dst = (ctx.instr_reg >> 3) & 7;

    set_register((Register)dst, mem_read(ctx.addr));
}

void CPU::MOV_M_r()
{
    int src = ctx.instr_reg & 7;

    mem_write(ctx.addr, read_register((Register)src));
}

void CPU::MVI_r()
{
    int dst = (ctx.instr_reg >> 3) & 7;

    set_register((Register)dst, ctx.data);
}

void CPU::MVI_M()
{
    AM_IND();

    mem_write(ctx.addr, ctx.data);
}

void CPU::LXI()
{
    set_register(instructions[ctx.instr_reg].reg, ctx.data);
}

void CPU::LDA()
{
    ctx.regs.a = mem_read(ctx.addr);
}

void CPU::STA()
{
    mem_write(ctx.addr, ctx.regs.a);
}

void CPU::LHLD()
{
    ctx.regs.l = mem_read(ctx.addr);
    ctx.regs.h = mem_read(ctx.addr + 1);
}

void CPU::SHLD()
{
    mem_write(ctx.addr, ctx.regs.l);
    mem_write(ctx.addr + 1, ctx.regs.h);
}

void CPU::LDAX()
{
    ctx.regs.a = mem_read(ctx.addr);
}

void CPU::STAX()
{
    mem_write(ctx.addr, ctx.regs.a);
}

void CPU::XCHG()
{
    u8 tmp = ctx.regs.h;
    ctx.regs.h = ctx.regs.d;
    ctx.regs.d = tmp;

    tmp = ctx.regs.l;
    ctx.regs.l = ctx.regs.e;
    ctx.regs.e = tmp;
}



// Arithmetic Group

void CPU::ADD()
{
    bool adc = false;
    u8 cy_bit = get_flag(FLAG_CY);

    if (ctx.instr_reg == 0x86 || ctx.instr_reg == 0x8E) // ADD M and ADC M instructions
    {
        ctx.data = mem_read(ctx.addr);
    }

    u8 result = ctx.regs.a + ctx.data;

    if ((ctx.instr_reg >= 0x88 && ctx.instr_reg <= 0x8F) || ctx.instr_reg == 0xCE) // ADC and ACI instructions
    {
        result += cy_bit;
        adc = true;
    }

    // (adc & cy_bit) is 1 ONLY when adding with carry AND the carry bit is set,
    // otherwise it is 0
    u8 cy = (ctx.regs.a & 0xFF) + (ctx.data & 0xFF) + (adc & cy_bit) > 0xFF;
    u8 ac = (ctx.regs.a & 0xF) + (ctx.data & 0xF) + (adc & cy_bit) > 0xF;

    update_flags_SZP(result);
    cy ? set_flag(FLAG_CY) : reset_flag(FLAG_CY);
    ac ? set_flag(FLAG_AC) : reset_flag(FLAG_AC);

    ctx.regs.a = result;
}

void CPU::SUB()
{
    bool sbb = false;
    u8 cy_bit = get_flag(FLAG_CY);

    if (ctx.instr_reg == 0x96 || ctx.instr_reg == 0x9E) // SUB M and SBB M instructions
    {
        ctx.data = mem_read(ctx.addr);
    }

    u8 result = ctx.regs.a - ctx.data;

    if ((ctx.instr_reg >= 0x98 && ctx.instr_reg <= 0x9F) || ctx.instr_reg == 0xDE) // SBB and SBI instructions
    {
        result -= cy_bit;
        sbb = true;
    }

    u8 cy = (ctx.regs.a & 0xFF) - (ctx.data & 0xFF) - (sbb & cy_bit) < 0;
    u8 ac = (ctx.regs.a & 0xF) - (ctx.data & 0xF) - (sbb & cy_bit) < 0;

    update_flags_SZP(result);
    cy ? set_flag(FLAG_CY) : reset_flag(FLAG_CY);
    ac ? set_flag(FLAG_AC) : reset_flag(FLAG_AC);

    ctx.regs.a = result;
}

void CPU::INR()
{
    if (ctx.instr_reg == 0x34) // INR M instruction
    {
        ctx.data = mem_read(ctx.addr);
    }

    u8 result = ctx.data + 1;

    if (ctx.instr_reg == 0x34)
    {
        mem_write(ctx.addr, result);
    }
    else
    {
        set_register(instructions[ctx.instr_reg].reg, result);
    }

    u8 ac = (ctx.data & 0xF) + 1 > 0xF;

    update_flags_SZP(result);
    ac ? set_flag(FLAG_AC) : reset_flag(FLAG_AC);
}

void CPU::DCR()
{
    if (ctx.instr_reg == 0x35) // DCR M instruction
    {
        ctx.data = mem_read(ctx.addr);
    }

    u8 result = ctx.data - 1;

    if (ctx.instr_reg == 0x35)
    {
        mem_write(ctx.addr, result);
    }
    else
    {
        set_register(instructions[ctx.instr_reg].reg, result);
    }

    u8 ac = (ctx.data & 0xF) - 1 < 0;

    update_flags_SZP(result);
    ac ? set_flag(FLAG_AC) : reset_flag(FLAG_AC);
}

void CPU::INX()
{
    u16 result = ctx.data + 1;

    set_register(instructions[ctx.instr_reg].reg, result);
}

void CPU::DCX()
{
    u16 result = ctx.data - 1;

    set_register(instructions[ctx.instr_reg].reg, result);
}

void CPU::DAD()
{
    u16 hl = read_register(REG_HL);
    u16 result = ctx.data + hl;
    set_register(REG_HL, result);

    u8 cy = (hl & 0xFFFF) + (ctx.data & 0xFFFF) > 0xFFFF;

    cy ? set_flag(FLAG_CY) : reset_flag(FLAG_CY);
}

void CPU::DAA()
{
    u8 ac_bit = get_flag(FLAG_AC) >> 4;
    u8 cy_bit = get_flag(FLAG_CY);
    u8 ac = 0;
    u8 cy = 0;

    if ((ctx.regs.a & 0xF) > 9 || ac_bit)
    {
        ac = ((ctx.regs.a & 0xF) + 6) > 0xF;
        ctx.regs.a += 6;

        if (((ctx.regs.a >> 4) & 0xF) > 9 || cy_bit)
        {
            cy = (ctx.regs.a + 0x60) > 0xFF;
            ctx.regs.a += 0x60;
        }
    }

    update_flags_SZP(ctx.regs.a);
    cy ? set_flag(FLAG_CY) : reset_flag(FLAG_CY);
    ac ? set_flag(FLAG_CY) : reset_flag(FLAG_CY);
}



// Logical Group

// NOTE: The CY Flag is cleared and AC is set to the OR'ing of bits 3 of the operands (8080)
//       (for both ANA and ANI)
// REF: MCS-80/85 Family User's Manual (October 1979)
void CPU::ANA_r()
{
    u8 val = ctx.data;

    u8 result = ctx.regs.a & val;
    ctx.regs.a = result;

    update_flags_SZP(result);
    reset_flag(FLAG_CY);
    u8 ac = ((ctx.regs.a & 0x8) | (val  & 0x8)) >> 3;
    ac ? set_flag(FLAG_AC) : reset_flag(FLAG_AC);
}

void CPU::ANA_M()
{
    u8 val = mem_read(ctx.addr);

    u8 result = ctx.regs.a & val;
    ctx.regs.a = result;

    update_flags_SZP(result);
    reset_flag(FLAG_CY);
    u8 ac = ((ctx.regs.a & 0x8) | (val  & 0x8)) >> 3;
    ac ? set_flag(FLAG_AC) : reset_flag(FLAG_AC);
}

// NOTE: One manual says AC flag is cleared, other says its set??
void CPU::ANI()
{
    u8 val = ctx.data;

    u8 result = ctx.regs.a & val;
    ctx.regs.a = result;

    update_flags_SZP(result);
    reset_flag(FLAG_CY);
    u8 ac = ((ctx.regs.a & 0x8) | (val  & 0x8)) >> 3;
    ac ? set_flag(FLAG_AC) : reset_flag(FLAG_AC);
}

void CPU::XRA_r()
{
    u8 val = ctx.data;

    u8 result = ctx.regs.a ^ val;
    ctx.regs.a = result;

    update_flags_SZP(result);
    reset_flag(FLAG_CY);
    reset_flag(FLAG_AC);
}

void CPU::XRA_M()
{
    u8 val = mem_read(ctx.addr);

    u8 result = ctx.regs.a ^ val;
    ctx.regs.a = result;

    update_flags_SZP(result);
    reset_flag(FLAG_CY);
    reset_flag(FLAG_AC);
}

void CPU::XRI()
{
    u8 val = ctx.data;

    u8 result = ctx.regs.a ^ val;
    ctx.regs.a = result;

    update_flags_SZP(result);
    reset_flag(FLAG_CY);
    reset_flag(FLAG_AC);
}

void CPU::ORA_r()
{
    u8 val = ctx.data;

    u8 result = ctx.regs.a | val;
    ctx.regs.a = result;

    update_flags_SZP(result);
    reset_flag(FLAG_CY);
    reset_flag(FLAG_AC);
}

void CPU::ORA_M()
{
    u8 val = mem_read(ctx.addr);

    u8 result = ctx.regs.a | val;
    ctx.regs.a = result;

    update_flags_SZP(result);
    reset_flag(FLAG_CY);
    reset_flag(FLAG_AC);
}

void CPU::ORI()
{
    u8 val = ctx.data;

    u8 result = ctx.regs.a | val;
    ctx.regs.a = result;

    update_flags_SZP(result);
    reset_flag(FLAG_CY);
    reset_flag(FLAG_AC);
}

void CPU::CMP_r()
{
    u8 val = ctx.data;

    u8 result = ctx.regs.a - val;

    update_flags_SZP(result);
    u8 cy = ctx.regs.a < val;
    u8 ac = (ctx.regs.a & 0xF) < (val & 0xF);
    cy ? set_flag(FLAG_CY) : reset_flag(FLAG_CY);
    ac ? set_flag(FLAG_AC) : reset_flag(FLAG_AC);
}

void CPU::CMP_M()
{
    u8 val = mem_read(ctx.addr);

    u8 result = ctx.regs.a - val;

    update_flags_SZP(result);
    u8 cy = ctx.regs.a < val;
    u8 ac = (ctx.regs.a & 0xF) < (val & 0xF);
    cy ? set_flag(FLAG_CY) : reset_flag(FLAG_CY);
    ac ? set_flag(FLAG_AC) : reset_flag(FLAG_AC);
}

void CPU::CPI()
{
    u8 val = ctx.data;

    u8 result = ctx.regs.a - val;

    update_flags_SZP(result);
    u8 cy = ctx.regs.a < val;
    u8 ac = (ctx.regs.a & 0xF) < (val & 0xF);
    cy ? set_flag(FLAG_CY) : reset_flag(FLAG_CY);
    ac ? set_flag(FLAG_AC) : reset_flag(FLAG_AC);
}

void CPU::RLC()
{
    u8 a7 = ctx.regs.a >> 7;

    ctx.regs.a <<= 1;
    ctx.regs.a |= a7;

    u8 cy = a7;
    cy ? set_flag(FLAG_CY) : reset_flag(FLAG_CY);
}

void CPU::RRC()
{
    u8 a0 = ctx.regs.a & 1;

    ctx.regs.a >>= 1;
    ctx.regs.a |= (a0 << 7);

    u8 cy = a0;
    cy ? set_flag(FLAG_CY) : reset_flag(FLAG_CY);
}

void CPU::RAL()
{
    u8 cy_bit = get_flag(FLAG_CY);
    u8 a7 = ctx.regs.a >> 7;

    ctx.regs.a <<= 1;
    ctx.regs.a |= cy_bit;

    u8 cy = a7;
    cy ? set_flag(FLAG_CY) : reset_flag(FLAG_CY);
}

void CPU::RAR()
{
    u8 cy_bit = get_flag(FLAG_CY);
    u8 a0 = ctx.regs.a & 1;

    ctx.regs.a >>= 1;
    ctx.regs.a |= (cy_bit << 7);

    u8 cy = a0;
    cy ? set_flag(FLAG_CY) : reset_flag(FLAG_CY);
}

void CPU::CMA()
{
    ctx.regs.a = ~ctx.regs.a;
}

void CPU::CMC()
{
    u8 cy = get_flag(FLAG_CY);
    cy = ~cy;

    cy ? set_flag(FLAG_CY) : reset_flag(FLAG_CY);
}

void CPU::STC()
{
    set_flag(FLAG_CY);
}



// Branch Group

void CPU::JMP()
{
    ctx.regs.pc = ctx.data;
}

void CPU::JMP_cond()
{
    u8 cond = (ctx.instr_reg >> 3) & 7;

    u8 z = get_flag(FLAG_Z) >> 6;
    u8 cy = get_flag(FLAG_CY);
    u8 p = get_flag(FLAG_P) >> 2;
    u8 s = get_flag(FLAG_S) >> 7;

    switch (cond)
    {
        case 0: if (!z) JMP();
        case 1: if (z)  JMP();
        case 2: if (!cy) JMP();
        case 3: if (cy)  JMP();
        case 4: if (!p) JMP();
        case 5: if (p)  JMP();
        case 6: if (!s) JMP();
        case 7: if (s)  JMP();
    }
}

void CPU::CALL()
{
    u8 pch = ctx.regs.pc >> 8;
    u8 pcl = ctx.regs.pc & 0xFF;

    mem_write(ctx.regs.sp - 1, pch);
    mem_write(ctx.regs.sp - 2, pcl);
    ctx.regs.sp -= 2;

    ctx.regs.pc = ctx.data;
}

void CPU::CALL_cond()
{
    u8 cond = (ctx.instr_reg >> 3) & 7;

    u8 z = get_flag(FLAG_Z) >> 6;
    u8 cy = get_flag(FLAG_CY);
    u8 p = get_flag(FLAG_P) >> 2;
    u8 s = get_flag(FLAG_S) >> 7;

    switch (cond)
    {
        case 0: if (!z) CALL();
        case 1: if (z)  CALL();
        case 2: if (!cy) CALL();
        case 3: if (cy)  CALL();
        case 4: if (!p) CALL();
        case 5: if (p)  CALL();
        case 6: if (!s) CALL();
        case 7: if (s)  CALL();
    }
}

void CPU::RET()
{
    u8 lo = mem_read(ctx.regs.sp);
    u8 hi = mem_read(ctx.regs.sp + 1);
    ctx.regs.sp += 2;

    ctx.regs.pc = (hi << 8) | lo;
}

void CPU::RET_cond()
{
    u8 cond = (ctx.instr_reg >> 3) & 7;

    u8 z = get_flag(FLAG_Z) >> 6;
    u8 cy = get_flag(FLAG_CY);
    u8 p = get_flag(FLAG_P) >> 2;
    u8 s = get_flag(FLAG_S) >> 7;

    switch (cond)
    {
        case 0: if (!z) RET();
        case 1: if (z)  RET();
        case 2: if (!cy) RET();
        case 3: if (cy)  RET();
        case 4: if (!p) RET();
        case 5: if (p)  RET();
        case 6: if (!s) RET();
        case 7: if (s)  RET();
    }
}

void CPU::RST()
{
    if (ctx.INTE)
    {
        u8 pch = (ctx.regs.pc >> 8) & 0xFF;
        u8 pcl = ctx.regs.pc & 0xFF;

        mem_write(ctx.regs.sp - 1, pch);
        mem_write(ctx.regs.sp - 2, pcl);
        ctx.regs.sp -= 2;

        u8 NNN = (ctx.instr_reg >> 3) & 0x07;

        ctx.regs.pc = 8 * NNN;

        ctx.INTE = 0;
    }
}

void CPU::PCHL()
{
    ctx.regs.pc = (ctx.regs.h << 8) | ctx.regs.l;
}



// Stack, I/O, and Machine Control Group

void CPU::PUSH()
{
    u8 rh = ctx.addr >> 7;
    u8 rl = ctx.addr & 0xFF;

    mem_write(ctx.regs.sp - 1, rh);
    mem_write(ctx.regs.sp - 2, rl);
    ctx.regs.sp -= 2;
}

void CPU::POP()
{
    u8 rl = mem_read(ctx.regs.sp);
    u8 rh = mem_read(ctx.regs.sp + 1);
    ctx.regs.sp += 2;

    set_register(instructions[ctx.instr_reg].reg, ((rh << 8) | rl));
}

void CPU::XTHL()
{
    u8 tmp = ctx.regs.l;
    ctx.regs.l = mem_read(ctx.regs.sp);
    mem_write(ctx.regs.sp, tmp);

    tmp = ctx.regs.h;
    ctx.regs.h = mem_read(ctx.regs.sp + 1);
    mem_write(ctx.regs.sp + 1, tmp);
}

void CPU::SPHL()
{
    ctx.regs.sp = ctx.data;
}

void CPU::IN()
{

}

void CPU::OUT()
{

}

void CPU::EI()
{
    ctx.INTE = 1;
}

void CPU::DI()
{
    ctx.INTE = 0;
}

void CPU::HLT()
{
    ctx.halted = 1;
}

void CPU::NOP()
{
}
