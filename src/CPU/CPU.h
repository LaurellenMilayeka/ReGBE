//
// Created by ML52298126 on 15/07/2022.
//

#ifndef GBE_CPU_H
#define GBE_CPU_H

#include <bitset>
#include <map>
#include <stack>

typedef void (*OpCodeExec)(void);

union Registry
{
    std::uint16_t reg;
    struct {
        std::uint8_t low;
        std::uint8_t high;
    };

    Registry(std::uint16_t i);
};

class CPU {

private:

    static std::map<std::uint8_t, OpCodeExec> opCodeExecList;
    static std::map<std::uint8_t, OpCodeExec> opCodeCBExecList;

public:

    static std::uint8_t ticks;
    static std::uint8_t cycles;

    static Registry af;
    static Registry bc;
    static Registry de;
    static Registry hl;

    static Registry sp;
    static std::uint16_t pc;

    static std::stack<std::uint16_t> callStack;

    static void ReadNextInstruction(std::uint8_t opcode);
    static void CBRedirect();
    static void PrintRegisters();
};


#endif //GBE_CPU_H
