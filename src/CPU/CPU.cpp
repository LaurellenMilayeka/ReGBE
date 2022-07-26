//
// Created by ML52298126 on 15/07/2022.
//

#include "CPU.h"

#include <CPU/Disasm.h>
#include <cstdio>

std::uint32_t CPU::overallClock = 0;

Registry CPU::af = 0;
Registry CPU::bc = 0;
Registry CPU::de = 0;
Registry CPU::hl = 0;

Registry CPU::sp = 0;
Registry CPU::pc = 0;

std::uint8_t CPU::ticks = 0;
std::uint8_t CPU::cycles = 0;

std::map<std::uint8_t, OpCodeExec> CPU::opCodeExecList = {
        {0x00, &Op0x00},
        {0x01, &Op0x01},
        {0x04, &Op0x04},
        {0x05, &Op0x05},
        {0x06, &Op0x06},
        {0x09, &Op0x09},
        {0x0B, &Op0x0B},
        {0x0C, &Op0x0C},
        {0x0D, &Op0x0D},
        {0x0E, &Op0x0E},
        {0x11, &Op0x11},
        {0x12, &Op0x12},
        {0x13, &Op0x13},
        {0x15, &Op0x15},
        {0x16, &Op0x16},
        {0x17, &Op0x17},
        {0x18, &Op0x18},
        {0x19, &Op0x19},
        {0x1A, &Op0x1A},
        {0x1C, &Op0x1C},
        {0x1E, &Op0x1E},
        {0x1D, &Op0x1D},
        {0x20, &Op0x20},
        {0x21, &Op0x21},
        {0x22, &Op0x22},
        {0x23, &Op0x23},
        {0x24, &Op0x24},
        {0x28, &Op0x28},
        {0x2A, &Op0x2A},
        {0x2E, &Op0x2E},
        {0x2F, &Op0x2F},
        {0x31, &Op0x31},
        {0x32, &Op0x32},
        {0x36, &Op0x36},
        {0x3D, &Op0x3D},
        {0x3E, &Op0x3E},
        {0x47, &Op0x47},
        {0x4F, &Op0x4F},
        {0x56, &Op0x56},
        {0x57, &Op0x57},
        {0x5E, &Op0x5E},
        {0x5F, &Op0x5F},
        {0x67, &Op0x67},
        {0x77, &Op0x77},
        {0x78, &Op0x78},
        {0x79, &Op0x79},
        {0x7B, &Op0x7B},
        {0x7C, &Op0x7C},
        {0x7D, &Op0x7D},
        {0x86, &Op0x86},
        {0x87, &Op0x87},
        {0x90, &Op0x90},
        {0xA1, &Op0xA1},
        {0xA7, &Op0xA7},
        {0xA9, &Op0xA9},
        {0xAF, &Op0xAF},
        {0xB0, &Op0xB0},
        {0xB1, &Op0xB1},
        {0xBE, &Op0xBE},
        {0xCB, &CBRedirect},
        {0xC1, &Op0xC1},
        {0xC3, &Op0xC3},
        {0xC5, &Op0xC5},
        {0xC8, &Op0xC8},
        {0xC9, &Op0xC9},
        {0xCA, &Op0xCA},
        {0xCD, &Op0xCD},
        {0xD1, &Op0xD1},
        {0xD5, &Op0xD5},
        {0xE0, &Op0xE0},
        {0xE1, &Op0xE1},
        {0xE2, &Op0xE2},
        {0xE5, &Op0xE5},
        {0xE6, &Op0xE6},
        {0xE9, &Op0xE9},
        {0xEA, &Op0xEA},
        {0xEF, &Op0xEF},
        {0xF0, &Op0xF0},
        {0xF3, &Op0xF3},
        {0xF5, &Op0xF5},
        {0xFA, &Op0xFA},
        {0xFB, &Op0xFB},
        {0xFE, &Op0xFE}
};

std::map<std::uint8_t, OpCodeExec> CPU::opCodeCBExecList = {
        {0x11, &OpCB0x11},
        {0x27, &OpCB0x27},
        {0x37, &OpCB0x37},
        {0x7C, &OpCB0x7C},
        {0x87, &OpCB0x87}
};

void CPU::ReadNextInstruction(std::uint8_t opcode) {
    auto it = CPU::opCodeExecList.find(opcode);
    if (it != CPU::opCodeExecList.end()) {
        it->second();
    }
    else {
        printf("Error at OpCode : 0x%02X (Not Implemented)", opcode);
        exit(EXIT_FAILURE);
    }
}

void CPU::PrintRegisters() {
    printf("Printing registers..\n");
    printf("AF : 0x%04X BC : 0x%04X\n", CPU::af, CPU::bc);
    printf("DE : 0x%04X HL : 0x%04X\n", CPU::de, CPU::hl);
    printf("SP : 0x%04X PC : 0x%04X\n", CPU::sp, CPU::pc);
}

void CPU::CBRedirect() {
    std::uint8_t opCode = RAM::At(CPU::pc.reg++);
    auto it = CPU::opCodeCBExecList.find(opCode);
    if (it != CPU::opCodeCBExecList.end()) {
        it->second();
    }
    else {
        printf("Error at OpCode (CB) : 0x%02X (Not Implemented)", opCode);
        exit(EXIT_FAILURE);
    }
}

Registry::Registry(std::uint16_t i) {
    reg = i;
}
