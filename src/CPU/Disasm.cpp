//
// Created by ML52298126 on 16/07/2022.
//

#include "Disasm.h"

void Op0x00()
{
    CPU::ticks  = 4;
    CPU::cycles = 1;
}

void Op0x01()
{
    CPU::bc.low = RAM::At(CPU::pc.reg++);
    CPU::bc.high = RAM::At(CPU::pc.reg++);

    CPU::ticks  = 12;
    CPU::cycles = 3;
}

void Op0x04()
{
    if ( ( ( (CPU::bc.high & 0x0F) + 0x01) & 0x10) == 0x10)
        CPU::af.low |= 0x20;
    else
        CPU::af.low &= ~(0x20);

    CPU::bc.high++;

    if (CPU::bc.high == 0x00)
        CPU::af.low |= 0x80;
    else
        CPU::af.low &= ~(0x80);
    CPU::af.low &= ~(0x40);

    CPU::ticks  = 4;
    CPU::cycles = 1;
}

void Op0x05()
{
    if ((((CPU::bc.high & 0x0F) - (0x01 & 0x0F)) & 0x10) == 0x10)
        CPU::af.low |= 0x20;
    else
        CPU::af.low &= ~(0x20);

    CPU::bc.high--;

    if (CPU::bc.high == 0x00)
        CPU::af.low |= 0x80;
    else
        CPU::af.low &= ~(0x80);
    CPU::af.low |= 0x40;

    CPU::ticks  = 4;
    CPU::cycles = 1;
}

void Op0x06()
{
    CPU::bc.high = RAM::At(CPU::pc.reg++);

    CPU::ticks  = 8;
    CPU::cycles = 2;
}

void Op0x09()
{
    if ((((CPU::hl.reg & 0x0FFF) + (CPU::bc.reg & 0x0FFF)) & 0x1000) == 0x1000)
        CPU::af.low |= 0x20;
    else
        CPU::af.low &= ~(0x20);

    if (((CPU::hl.reg + CPU::bc.reg) & 0x10000) == 0x10000)
        CPU::af.low |= 0x10;
    else
        CPU::af.low &= ~(0x10);

    CPU::hl.reg += CPU::bc.reg;

    CPU::af.low &= ~(0x40);
}

void Op0x0B()
{
    CPU::bc.reg--;

    CPU::ticks  = 8;
    CPU::cycles = 2;
}

void Op0x0C()
{
    if ((((CPU::bc.low & 0x0F) + (0x01 & 0x0F)) & 0x10) == 0x10)
        CPU::af.low |= 0x20;
    else
        CPU::af.low &= ~(0x20);

    CPU::bc.low++;

    if (CPU::bc.low == 0x00)
        CPU::af.low |= 0x80;
    else
        CPU::af.low &= ~(0x80);
    CPU::af.low &= ~(0x40);

    CPU::ticks  = 4;
    CPU::cycles = 1;
}

void Op0x0D()
{
    if ((((CPU::bc.low & 0x0F) - (0x01 & 0x0F)) & 0x10) == 0x10)
        CPU::af.low |= 0x20;
    else
        CPU::af.low &= ~(0x20);

    CPU::bc.low--;

    if (CPU::bc.low == 0x00)
        CPU::af.low |= 0x80;
    else
        CPU::af.low &= ~(0x80);
    CPU::af.low |= 0x40;

    CPU::ticks  = 4;
    CPU::cycles = 1;
}

void Op0x0E()
{
    CPU::bc.low = RAM::At(CPU::pc.reg++);

    CPU::ticks  = 8;
    CPU::cycles = 2;
}

void Op0x11()
{
    CPU::de.reg = RAM::At(CPU::pc.reg + 1) << 8 | (RAM::At(CPU::pc.reg));
    CPU::pc.reg += 2;

    CPU::ticks  = 12;
    CPU::cycles = 3;
}

void Op0x12()
{
    RAM::SetAt(CPU::de.reg, CPU::af.high);

    CPU::ticks  = 8;
    CPU::cycles = 2;
}

void Op0x13()
{
    CPU::de.reg++;

    CPU::ticks  = 8;
    CPU::cycles = 2;
}

void Op0x15()
{
    if ((((CPU::de.high & 0x0F) - (0x01 & 0x0F)) & 0x10) == 0x10)
        CPU::af.low |= 0x20;
    else
        CPU::af.low &= ~(0x20);

    CPU::de.high--;

    if (CPU::de.high == 0x00)
        CPU::af.low |= 0x80;
    else
        CPU::af.low &= ~(0x80);
    CPU::af.low |= 0x40;

    CPU::ticks  = 4;
    CPU::cycles = 1;
}

void Op0x16()
{
    CPU::de.high = RAM::At(CPU::pc.reg++);

    CPU::ticks  = 8;
    CPU::cycles = 2;
}

void Op0x17()
{
    std::uint8_t newCarry = ((CPU::af.high & 0x80) == 0x80) ? 1 : 0;
    CPU::af.high = CPU::af.high << 1 | (((CPU::af.low & 0x10) == 0x10) ? 1 & 0x01 : 0 & 0x01);
    if (newCarry == 1)
        CPU::af.low |= 0x10;
    else
        CPU::af.low &= ~(0x10);
    CPU::af.low &= ~(0x80);
    CPU::af.low &= ~(0x40);
    CPU::af.low &= ~(0x20);

    CPU::ticks  = 4;
    CPU::cycles = 1;
}

void Op0x18()
{
    auto jmp = static_cast<std::int8_t>(RAM::At(CPU::pc.reg));
    CPU::pc.reg += (jmp + 1);

    CPU::ticks  = 12;
    CPU::cycles = 3;
}

void Op0x19()
{
    if ((((CPU::hl.reg & 0x0FFF) + (CPU::de.reg & 0x0FFF)) & 0x1000) == 0x1000)
        CPU::af.low |= 0x20;
    else
        CPU::af.low &= ~(0x20);

    if ((((CPU::hl.reg & 0xFFFF) + (CPU::de.reg & 0xFFFF)) & 0x10000) == 0x10000)
        CPU::af.low |= 0x10;
    else
        CPU::af.low &= ~(0x10);

    CPU::hl.reg += CPU::de.reg;

    if (CPU::hl.reg == 0x00)
        CPU::af.low |= 0x80;
    else
        CPU::af.low &= ~(0x80);
    CPU::af.low &= ~(0x40);

    CPU::ticks  = 8;
    CPU::cycles = 2;
}

void Op0x1A()
{
    CPU::af.high = RAM::At(CPU::de.reg);

    CPU::ticks  = 8;
    CPU::cycles = 2;
}

void Op0x1C()
{
    if ((((CPU::de.low & 0x0F) + (0x01 & 0x0F)) & 0x10) == 0x10)
        CPU::af.low |= 0x20;
    else
        CPU::af.low &= ~(0x20);

    CPU::de.low++;

    if (CPU::de.low == 0x00)
        CPU::af.low |= 0x80;
    else
        CPU::af.low &= ~(0x80);
    CPU::af.low &= ~(0x40);

    CPU::ticks  = 4;
    CPU::cycles = 1;
}

void Op0x1D()
{
    if ((((CPU::de.low & 0x0F) - (0x01 & 0x0F)) & 0x10) == 0x10)
        CPU::af.low |= 0x20;
    else
        CPU::af.low &= ~(0x20);

    CPU::de.low--;

    if (CPU::de.low == 0x00)
        CPU::af.low |= 0x80;
    else
        CPU::af.low &= ~(0x80);
    CPU::af.low |= 0x40;

    CPU::ticks  = 4;
    CPU::cycles = 1;
}

void Op0x1E()
{
    CPU::de.low = RAM::At(CPU::pc.reg++);

    CPU::ticks  = 8;
    CPU::cycles = 2;
}

void Op0x20()
{
    if ((CPU::af.low & 0x80) != 0x80) {
        auto jmp = static_cast<std::int8_t>(RAM::At(CPU::pc.reg));
        CPU::pc.reg += (jmp + 1);

        CPU::ticks  = 12;
        CPU::cycles = 3;
    } else {
        CPU::pc.reg += 1;

        CPU::ticks  = 8;
        CPU::cycles = 2;
    }
}

void Op0x21()
{
    CPU::hl.reg = RAM::At(CPU::pc.reg + 1) << 8 | RAM::At(CPU::pc.reg);
    CPU::pc.reg += 2;

    CPU::ticks  = 12;
    CPU::cycles = 3;
}

void Op0x22()
{
    RAM::SetAt(CPU::hl.reg, CPU::af.high);
    CPU::hl.reg++;

    CPU::ticks  = 8;
    CPU::cycles = 2;
}

void Op0x23()
{
    CPU::hl.reg++;

    CPU::ticks  = 8;
    CPU::cycles = 2;
}

void Op0x24()
{
    if ((((CPU::hl.high & 0x0F) + (0x01 & 0x0F)) & 0x10) == 0x10)
        CPU::af.low |= 0x20;
    else
        CPU::af.low &= ~(0x20);

    CPU::hl.high++;

    if (CPU::hl.high == 0x00)
        CPU::af.low |= 0x80;
    else
        CPU::af.low &= ~(0x80);
    CPU::af.low &= ~(0x40);

    CPU::ticks  = 4;
    CPU::cycles = 1;
}

void Op0x28()
{
    if ((CPU::af.low & 0x80) == 0x80) {
        auto jmp = static_cast<std::int8_t>(RAM::At(CPU::pc.reg));
        CPU::pc.reg += (jmp + 1);

        CPU::ticks  = 12;
        CPU::cycles = 3;
    } else {
        CPU::pc.reg += 1;

        CPU::ticks  = 8;
        CPU::cycles = 2;
    }
}

void Op0x2A()
{
    CPU::af.high = RAM::At(CPU::hl.reg);
    CPU::hl.reg++;

    CPU::ticks  = 8;
    CPU::cycles = 2;
}

void Op0x2E()
{
    CPU::hl.low = RAM::At(CPU::pc.reg++);

    CPU::ticks  = 8;
    CPU::cycles = 2;
}

void Op0x2F()
{
    CPU::af.high = ~(CPU::af.high);
    CPU::af.low |= 0x40;
    CPU::af.low |= 0x20;

    CPU::ticks  = 4;
    CPU::cycles = 1;
}

void Op0x31() {
    CPU::sp.reg = RAM::At(CPU::pc.reg + 1) << 8 | RAM::At(CPU::pc.reg);
    CPU::pc.reg += 2;

    CPU::ticks  = 12;
    CPU::cycles = 3;
}

void Op0x32()
{
    RAM::SetAt(CPU::hl.reg, CPU::af.high);
    CPU::hl.reg -= 1;

    CPU::ticks  = 8;
    CPU::cycles = 2;
}

void Op0x36()
{
    RAM::SetAt(CPU::hl.reg, RAM::At(CPU::pc.reg++));

    CPU::ticks  = 12;
    CPU::cycles = 3;
}

void Op0x3D()
{
    if ((((CPU::af.high & 0x0F) - (0x01 & 0x0F)) & 0x10) == 0x10)
        CPU::af.low |= 0x20;
    else
        CPU::af.low &= ~(0x20);

    CPU::af.high--;

    if (CPU::af.high == 0x00)
        CPU::af.low |= 0x80;
    else
        CPU::af.low &= ~(0x80);
    CPU::af.low |= 0x40;

    CPU::ticks  = 4;
    CPU::cycles = 1;
}

void Op0x3E()
{
    CPU::af.high = RAM::At(CPU::pc.reg++);

    CPU::ticks  = 8;
    CPU::cycles = 2;
}

void Op0x47()
{
    CPU::bc.high = CPU::af.high;

    CPU::ticks  = 4;
    CPU::cycles = 1;
}

void Op0x4F()
{
    CPU::bc.low = CPU::af.high;

    CPU::ticks  = 4;
    CPU::cycles = 1;
}

void Op0x56()
{
    CPU::de.high = RAM::At(CPU::hl.reg);

    CPU::ticks = 8;
    CPU::cycles = 2;
}

void Op0x57()
{
    CPU::de.high = CPU::af.high;

    CPU::ticks  = 4;
    CPU::cycles = 1;
}

void Op0x5E()
{
    CPU::de.low = RAM::At(CPU::hl.reg);

    CPU::ticks  = 8;
    CPU::cycles = 2;
}

void Op0x5F()
{
    CPU::de.low = CPU::af.high;

    CPU::ticks  = 4;
    CPU::cycles = 1;
}

void Op0x67()
{
    CPU::hl.high = CPU::af.high;

    CPU::ticks  = 4;
    CPU::cycles = 1;
}

void Op0x77()
{
    RAM::SetAt(CPU::hl.reg, CPU::af.high);

    CPU::ticks  = 8;
    CPU::cycles = 2;
}

void Op0x78()
{
    CPU::af.high = CPU::bc.high;

    CPU::ticks  = 4;
    CPU::cycles = 1;
}

void Op0x79()
{
    CPU::af.high = CPU::bc.low;

    CPU::ticks  = 4;
    CPU::cycles = 1;
}

void Op0x7B()
{
    CPU::af.high = CPU::de.low;

    CPU::ticks  = 4;
    CPU::cycles = 1;
}

void Op0x7C()
{
    CPU::af.high = CPU::hl.high;

    CPU::ticks  = 4;
    CPU::cycles = 1;
}

void Op0x7D()
{
    CPU::af.high = CPU::hl.low;

    CPU::ticks  = 4;
    CPU::cycles = 1;
}

void Op0x86()
{
    if ((((CPU::af.high & 0x0F) + (RAM::At(CPU::hl.reg) & 0x0F)) & 0x10) == 0x10)
        CPU::af.low |= 0x20;
    else
        CPU::af.low &= ~(0x20);

    if ((((CPU::af.high & 0xFF) + (RAM::At(CPU::hl.reg) & 0xFF)) & 0x100) == 0x100)
        CPU::af.low |= 0x10;
    else
        CPU::af.low &= ~(0x10);

    CPU::af.high += RAM::At(CPU::hl.reg);

    if (CPU::af.high == 0x00)
        CPU::af.low |= 0x80;
    else
        CPU::af.low &= ~(0x80);
    CPU::af.low &= ~(0x40);

    CPU::ticks  = 8;
    CPU::cycles = 2;
}

void Op0x87()
{
    if ((((CPU::af.high & 0x0F) + (CPU::af.high & 0x0F)) & 0x10) == 0x10)
        CPU::af.low |= 0x20;
    else
        CPU::af.low &= ~(0x20);

    if ((((CPU::af.high & 0xFF) + (CPU::af.high & 0xFF)) & 0x100) == 0x100)
        CPU::af.low |= 0x10;
    else
        CPU::af.low &= ~(0x10);

    CPU::af.high += CPU::af.high;

    if (CPU::af.high == 0x00)
        CPU::af.low |= 0x80;
    else
        CPU::af.low &= ~(0x80);
    CPU::af.low &= ~(0x40);

    CPU::ticks  = 4;
    CPU::cycles = 1;
}

void Op0x90()
{
    if ((((CPU::bc.high & 0x0F) - (CPU::af.high & 0x0F)) & 0x10) == 0x10)
        CPU::af.low |= 0x20;
    else
        CPU::af.low &= ~(0x20);

    if ((((CPU::bc.high & 0xFF) - (CPU::af.high & 0xFF)) & 0x100) == 0x100)
        CPU::af.low |= 0x10;
    else
        CPU::af.low &= ~(0x10);

    CPU::af.high -= CPU::bc.high;

    if (CPU::af.high == 0x00)
        CPU::af.low |= 0x80;
    else
        CPU::af.low &= ~(0x80);
    CPU::af.low |= 0x40;

    CPU::ticks  = 4;
    CPU::cycles = 1;
}

void Op0xA1()
{
    CPU::af.high &= CPU::bc.low;

    if (CPU::af.high == 0x00)
        CPU::af.low |= 0x80;
    else
        CPU::af.low &= ~(0x80);
    CPU::af.low &= ~(0x40);
    CPU::af.low |= 0x20;
    CPU::af.low &= ~(0x10);

    CPU::ticks  = 4;
    CPU::cycles = 1;
}

void Op0xA7()
{
    CPU::af.high &= CPU::af.high;

    if (CPU::af.high == 0x00)
        CPU::af.low |= 0x80;
    else
        CPU::af.low &= ~(0x80);
    CPU::af.low &= ~(0x40);
    CPU::af.low |= 0x20;
    CPU::af.low &= ~(0x10);
}

void Op0xA9()
{
    CPU::af.high ^= CPU::bc.low;

    if (CPU::af.high == 0x00)
        CPU::af.low |= 0x80;
    else
        CPU::af.low &= ~(0x80);
    CPU::af.low &= ~(0x40);
    CPU::af.low &= ~(0x20);
    CPU::af.low &= ~(0x10);

    CPU::ticks  = 4;
    CPU::cycles = 1;
}

void Op0xAF()
{
    CPU::af.high ^= CPU::af.high;

    if (CPU::af.high == 0x00)
        CPU::af.low |= 0x80;
    else
        CPU::af.low &= ~(0x80);
    CPU::af.low &= ~(0x40);
    CPU::af.low &= ~(0x20);
    CPU::af.low &= ~(0x10);

    CPU::ticks  = 4;
    CPU::cycles = 1;
}

void Op0xB0()
{
    CPU::af.high |= CPU::bc.high;

    if (CPU::af.high == 0x00)
        CPU::af.low |= 0x80;
    else
        CPU::af.low &= ~(0x80);
    CPU::af.low &= ~(0x40);
    CPU::af.low &= ~(0x20);
    CPU::af.low &= ~(0x10);

    CPU::ticks  = 4;
    CPU::cycles = 1;
}

void Op0xB1()
{
    CPU::af.high |= CPU::bc.low;

    if (CPU::af.high == 0x00)
        CPU::af.low |= 0x80;
    else
        CPU::af.low &= ~(0x80);
    CPU::af.low &= ~(0x40);
    CPU::af.low &= ~(0x20);
    CPU::af.low &= ~(0x10);

    CPU::ticks  = 4;
    CPU::cycles = 1;
}

void Op0xBE()
{
    if ((((RAM::At(CPU::hl.reg) & 0xFF) - (CPU::af.high & 0xFF)) & 0x100) == 0x100)
        CPU::af.low |= 0x10;
    else
        CPU::af.low &= ~(0x10);

    if ((((RAM::At(CPU::hl.reg) & 0x0F) - (CPU::af.high & 0x0F)) & 0x10) == 0x10)
        CPU::af.low |= 0x20;
    else
        CPU::af.low &= ~(0x20);

    if (CPU::af.high == RAM::At(CPU::hl.reg))
        CPU::af.low |= 0x80;
    else
        CPU::af.low &= ~(0x80);
    CPU::af.low |= 0x40;

    CPU::ticks  = 8;
    CPU::cycles = 2;
}

void Op0xC1()
{
    CPU::bc.low = RAM::At(CPU::sp.reg++);
    CPU::bc.high = RAM::At(CPU::sp.reg++);

    CPU::ticks  = 12;
    CPU::cycles = 3;
}

void Op0xC3()
{
    CPU::pc = RAM::At(CPU::pc.reg + 1) << 8 | RAM::At(CPU::pc.reg);

    CPU::ticks  = 16;
    CPU::cycles = 4;
}

void Op0xC5()
{
    RAM::SetAt(--CPU::sp.reg, CPU::bc.high);
    RAM::SetAt(--CPU::sp.reg, CPU::bc.low);

    CPU::ticks  = 16;
    CPU::cycles = 4;
}

void Op0xC8()
{
    if ((CPU::af.low & 0x80) == 0x80) {
        RAM::SetAt(--CPU::sp.reg, CPU::pc.high);
        RAM::SetAt(--CPU::sp.reg, CPU::pc.low);

        CPU::ticks  = 20;
        CPU::cycles = 5;
    }
    else {
        CPU::ticks  = 8;
        CPU::cycles = 2;
    }
}

void Op0xC9()
{
    CPU::pc.low = RAM::At(CPU::sp.reg++);
    CPU::pc.high = RAM::At(CPU::sp.reg++);

    CPU::ticks  = 16;
    CPU::cycles = 4;
}

void Op0xCA()
{
    if ((CPU::af.low & 0x80) == 0x80) {
        std::uint16_t newAddr = RAM::At(CPU::pc.reg + 1) << 8 | RAM::At(CPU::pc.reg);
        CPU::pc = newAddr;

        CPU::ticks  = 16;
        CPU::cycles = 4;
    }
    else {
        CPU::pc.reg += 2;

        CPU::ticks  = 12;
        CPU::cycles = 3;
    }
}

void Op0xCD()
{
    CPU::pc.reg += 2;

    RAM::SetAt(--CPU::sp.reg, CPU::pc.high);
    RAM::SetAt(--CPU::sp.reg, CPU::pc.low);

    CPU::pc = RAM::At(CPU::pc.reg - 1) << 8 | RAM::At(CPU::pc.reg - 2);

    CPU::ticks  = 24;
    CPU::cycles = 6;
}

void Op0xD1()
{
    CPU::de.low = RAM::At(CPU::sp.reg++);
    CPU::de.high = RAM::At(CPU::sp.reg++);

    CPU::ticks  = 12;
    CPU::cycles = 3;
}

void Op0xD5()
{
    RAM::SetAt(--CPU::sp.reg, CPU::de.high);
    RAM::SetAt(--CPU::sp.reg, CPU::de.low);

    CPU::ticks  = 16;
    CPU::cycles = 4;
}

void Op0xE0()
{
    std::uint16_t newAddr = 0xFF00 + RAM::At(CPU::pc.reg++);
    RAM::SetAt(newAddr, CPU::af.high);

    CPU::ticks  = 12;
    CPU::cycles = 3;
}

void Op0xE1()
{
    CPU::hl.low = RAM::At(CPU::sp.reg++);
    CPU::hl.high = RAM::At(CPU::sp.reg++);

    CPU::ticks  = 12;
    CPU::cycles = 3;
}

void Op0xE2()
{
    std::uint16_t newAddr = 0xFF00 + CPU::bc.low;
    RAM::SetAt(newAddr, CPU::af.high);

    CPU::ticks  = 8;
    CPU::cycles = 2;
}

void Op0xE5()
{
    RAM::SetAt(--CPU::sp.reg, CPU::hl.high);
    RAM::SetAt(--CPU::sp.reg, CPU::hl.low);

    CPU::ticks  = 16;
    CPU::cycles = 4;
}

void Op0xE6()
{
    CPU::af.high &= RAM::At(CPU::pc.reg++);

    if (CPU::af.high == 0x00)
        CPU::af.low |= 0x80;
    else
        CPU::af.low &= ~(0x80);
    CPU::af.low &= ~(0x40);
    CPU::af.low |= 0x20;
    CPU::af.low &= ~(0x10);

    CPU::ticks  = 8;
    CPU::cycles = 2;
}

void Op0xE9()
{
    CPU::pc = CPU::hl.reg;

    CPU::ticks  = 4;
    CPU::cycles = 1;
}

void Op0xEA()
{
    std::uint16_t newAddr = RAM::At(CPU::pc.reg + 1) << 8 | RAM::At(CPU::pc.reg);
    RAM::SetAt(newAddr, CPU::af.high);
    CPU::pc.reg += 2;

    CPU::ticks  = 16;
    CPU::cycles = 4;
}

void Op0xEF()
{
    RAM::SetAt(--CPU::sp.reg, CPU::pc.high);
    RAM::SetAt(--CPU::sp.reg, CPU::pc.low);
    CPU::pc = 0x0028;

    CPU::ticks  = 16;
    CPU::cycles = 4;
}

void Op0xF0()
{
    std::uint16_t newAddr = 0xFF00 | RAM::At(CPU::pc.reg++);
    CPU::af.high = RAM::At(newAddr);

    CPU::ticks  = 12;
    CPU::cycles = 3;
}

void Op0xF3()
{
    CPU::ticks  = 4;
    CPU::cycles = 1;
}

void Op0xF5()
{
    RAM::SetAt(--CPU::sp.reg, CPU::af.high);
    RAM::SetAt(--CPU::sp.reg, CPU::af.low);

    CPU::ticks  = 16;
    CPU::cycles = 4;
}

void Op0xFA()
{
    std::uint16_t newAddr = RAM::At(CPU::pc.reg + 1) << 8 | RAM::At(CPU::pc.reg);
    CPU::af.high = RAM::At(newAddr);

    CPU::pc.reg += 2;

    CPU::ticks  = 16;
    CPU::cycles = 4;
}

void Op0xFB()
{
    CPU::ticks  = 4;
    CPU::cycles = 1;
}

void Op0xFE()
{
    if ((((RAM::At(CPU::pc.reg) & 0xFF) - (CPU::af.high & 0xFF)) & 0x100) == 0x100)
        CPU::af.low |= 0x10;
    else
        CPU::af.low &= ~(0x10);

    if ((((RAM::At(CPU::pc.reg) & 0x0F) - (CPU::af.high & 0x0F)) & 0x10) == 0x10)
        CPU::af.low |= 0x20;
    else
        CPU::af.low &= ~(0x20);

    if (CPU::af.high == RAM::At(CPU::pc.reg++))
        CPU::af.low |= 0x80;
    else
        CPU::af.low &= ~(0x80);
    CPU::af.low |= 0x40;

    CPU::ticks  = 8;
    CPU::cycles = 2;
}

void OpCB0x11()
{
    std::uint8_t newCarry = ((CPU::bc.low & 0x80) == 0x80) ? 1 : 0;
    CPU::bc.low = CPU::bc.low << 1 | (((CPU::af.low & 0x10) == 0x10) ? 1 & 0x01 : 0 & 0x01);

    if (newCarry)
        CPU::af.low |= 0x10;
    else
        CPU::af.low &= ~0x10;

    if (CPU::bc.low == 0x00)
        CPU::af.low |= 0x80;
    else
        CPU::af.low &= ~(0x80);
    CPU::af.low &= ~(0x40);
    CPU::af.low &= ~(0x20);

    CPU::ticks  = 8;
    CPU::cycles = 2;
}

void OpCB0x27()
{
    std::uint8_t newCarry = ((CPU::bc.low & 0x80) == 0x80) ? 1 : 0;

    CPU::af.high <<= 1;

    if (CPU::af.high == 0x00)
        CPU::af.low |= 0x80;
    else
        CPU::af.low &= ~(0x80);

    if (newCarry)
        CPU::af.low |= 0x10;
    else
        CPU::af.low &= ~0x10;
    CPU::af.low &= ~(0x20);
    CPU::af.low &= ~(0x40);

    CPU::ticks  = 8;
    CPU::cycles = 2;
}

void OpCB0x37()
{
    CPU::af.high = (CPU::af.high & 0x0F) << 4 | (CPU::af.high & 0xF0) >> 4;

    if (CPU::af.high == 0x00)
        CPU::af.low |= 0x80;
    else
        CPU::af.low &= ~(0x80);
    CPU::af.low &= ~(0x40);
    CPU::af.low &= ~(0x20);
    CPU::af.low &= ~(0x10);

    CPU::ticks  = 8;
    CPU::cycles = 2;
}

void OpCB0x7C()
{
    if ((CPU::hl.high & 0x80) == 0x00)
        CPU::af.low |= 0x80;
    else
        CPU::af.low &= ~(0x80);
    CPU::af.low |= 0x20;
    CPU::af.low &= ~(0x40);

    CPU::ticks  = 8;
    CPU::cycles = 2;
}

void OpCB0x87()
{
    CPU::af.high &= ~(0x01);

    CPU::ticks  = 8;
    CPU::cycles = 2;
}