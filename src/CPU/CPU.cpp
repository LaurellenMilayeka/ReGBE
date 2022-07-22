//
// Created by ML52298126 on 15/07/2022.
//

#include "CPU.h"

#include <CPU/Disasm.h>
#include <cstdio>

Registry CPU::af = 0;
Registry CPU::bc = 0;
Registry CPU::de = 0;
Registry CPU::hl = 0;

Registry CPU::sp = 0;
std::uint16_t CPU::pc = 0;

std::uint8_t CPU::ticks = 0;
std::uint8_t CPU::cycles = 0;

std::map<std::uint8_t, OpCodeExec> CPU::opCodeExecList = {
        {0x04, &Op0x04},
        {0x05, &Op0x05},
        {0x06, &Op0x06},
        {0x0C, &Op0x0C},
        {0x0D, &Op0x0D},
        {0x0E, &Op0x0E},
        {0x11, &Op0x11},
        {0x13, &Op0x13},
        {0x15, &Op0x15},
        {0x16, &Op0x16},
        {0x17, &Op0x17},
        {0x18, &Op0x18},
        {0x1A, &Op0x1A},
        {0x1E, &Op0x1E},
        {0x1D, &Op0x1D},
        {0x20, &Op0x20},
        {0x21, &Op0x21},
        {0x22, &Op0x22},
        {0x23, &Op0x23},
        {0x24, &Op0x24},
        {0x28, &Op0x28},
        {0x2E, &Op0x2E},
        {0x31, &Op0x31},
        {0x32, &Op0x32},
        {0x3D, &Op0x3D},
        {0x3E, &Op0x3E},
        {0x4F, &Op0x4F},
        {0x57, &Op0x57},
        {0x67, &Op0x67},
        {0x77, &Op0x77},
        {0x78, &Op0x78},
        {0x7B, &Op0x7B},
        {0x7C, &Op0x7C},
        {0x7D, &Op0x7D},
        {0x86, &Op0x86},
        {0x90, &Op0x90},
        {0xAF, &Op0xAF},
        {0xBE, &Op0xBE},
        {0xCB, &CBRedirect},
        {0xC1, &Op0xC1},
        {0xC5, &Op0xC5},
        {0xC9, &Op0xC9},
        {0xCD, &Op0xCD},
        {0xE0, &Op0xE0},
        {0xE2, &Op0xE2},
        {0xEA, &Op0xEA},
        {0xF0, &Op0xF0},
        {0xFE, &Op0xFE}
};

std::map<std::uint8_t, OpCodeExec> CPU::opCodeCBExecList = {
        {0x11, &OpCB0x11},
        {0x7C, &OpCB0x7C}
};

std::stack<std::uint16_t> CPU::callStack = {};

void CPU::ReadNextInstruction(std::uint8_t opcode) {
    auto it = CPU::opCodeExecList.find(opcode);
    if (it != CPU::opCodeExecList.end()) {
        it->second();
    }
    else {
        printf("Error at OpCode : 0x%02X (Not Implemented)", opcode);
    }
}

void CPU::PrintRegisters() {
    printf("Printing registers..\n");
    printf("AF : 0x%04X BC : 0x%04X\n", CPU::af, CPU::bc);
    printf("DE : 0x%04X HL : 0x%04X\n", CPU::de, CPU::hl);
    printf("SP : 0x%04X PC : 0x%04X\n", CPU::sp, CPU::pc);

    printf("[0xFF05] : 0x%02X [0xFF06] : 0x%02X\n", RAM::At(0xFF05), RAM::At(0xFF06));
    printf("[0xFF07] : 0x%02X [0xFF10] : 0x%02X\n", RAM::At(0xFF07), RAM::At(0xFF10));
    printf("[0xFF11] : 0x%02X [0xFF12] : 0x%02X\n", RAM::At(0xFF11), RAM::At(0xFF12));
    printf("[0xFF14] : 0x%02X [0xFF16] : 0x%02X\n", RAM::At(0xFF14), RAM::At(0xFF16));
    printf("[0xFF17] : 0x%02X [0xFF19] : 0x%02X\n", RAM::At(0xFF17), RAM::At(0xFF19));
    printf("[0xFF1A] : 0x%02X [0xFF1B] : 0x%02X\n", RAM::At(0xFF1A), RAM::At(0xFF1B));
    printf("[0xFF1C] : 0x%02X [0xFF1E] : 0x%02X\n", RAM::At(0xFF1C), RAM::At(0xFF1E));
    printf("[0xFF20] : 0x%02X [0xFF21] : 0x%02X\n", RAM::At(0xFF20), RAM::At(0xFF21));
    printf("[0xFF22] : 0x%02X [0xFF23] : 0x%02X\n", RAM::At(0xFF22), RAM::At(0xFF23));
    printf("[0xFF24] : 0x%02X [0xFF25] : 0x%02X\n", RAM::At(0xFF24), RAM::At(0xFF25));
    printf("[0xFF26] : 0x%02X [0xFF40] : 0x%02X\n", RAM::At(0xFF26), RAM::At(0xFF40));
    printf("[0xFF42] : 0x%02X [0xFF43] : 0x%02X\n", RAM::At(0xFF42), RAM::At(0xFF43));
    printf("[0xFF45] : 0x%02X [0xFF47] : 0x%02X\n", RAM::At(0xFF45), RAM::At(0xFF47));
    printf("[0xFF48] : 0x%02X [0xFF49] : 0x%02X\n", RAM::At(0xFF48), RAM::At(0xFF49));
    printf("[0xFF4A] : 0x%02X [0xFF4B] : 0x%02X\n", RAM::At(0xFF4A), RAM::At(0xFF4B));
    printf("[0xFFFF] : 0x%02X\n", RAM::At(0xFFFF));
    printf("\n");
}

void CPU::CBRedirect() {
    std::uint8_t opCode = RAM::At(CPU::pc++);
    auto it = CPU::opCodeCBExecList.find(opCode);
    if (it != CPU::opCodeCBExecList.end()) {
        it->second();
    }
    else {
        printf("Error at OpCode (CB) : 0x%02X (Not Implemented)", opCode);
    }
}

Registry::Registry(std::uint16_t i) {
    reg = i;
}
