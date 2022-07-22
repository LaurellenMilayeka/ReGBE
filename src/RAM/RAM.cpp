//
// Created by ML52298126 on 15/07/2022.
//

#include "RAM.h"

#include <cstdio>
#include <iostream>
#include <fstream>

unsigned char RAM::_data[65535] = {};

bool RAM::LoadBIOS(const std::string &biosPath) {
    std::ifstream rom(biosPath, std::ios::binary);
    if (rom.is_open())
    {
        rom.seekg(0, std::ifstream::end);
        size_t romSize = rom.tellg();
        rom.seekg(0, std::ifstream::beg);
        rom.read(reinterpret_cast<char*>(&_data[0x0000]), romSize);

        for (unsigned int index = 0; index < romSize; index++)
        {
            if ((index % 16) == 0)
                printf("\n");
            printf("0x%02X ", _data[index]);
        }
        printf("\n");
    }
    else
    {
        std::cerr << "Could not open BIOS " << biosPath << std::endl;
    }
    return false;
}

bool RAM::LoadROM(const std::string &romPath) {
    std::ifstream rom(romPath, std::ios::binary);
    if (rom.is_open())
    {
        rom.seekg(0, std::ifstream::end);
        size_t romSize = rom.tellg();
        rom.seekg(0, std::ifstream::beg);
        rom.read(reinterpret_cast<char*>(&_data[0x0100]), romSize);
    }
    else
    {
        std::cerr << "Could not open ROM " << romPath << std::endl;
    }
    return false;
}

unsigned char RAM::At(std::uint16_t index) {
    return RAM::_data[index];
}

void RAM::SetAt(std::uint16_t index, std::uint8_t value) {
    RAM::_data[index] = value;
}

void RAM::DisplayCRC()
{
    printf("\nPrinting CRC field in ROM :");
    for (unsigned int index = 0x0000; index < 0x0150; index++)
    {
        if ((index % 16) == 0x00)
            printf("\n");
        printf("0x%02X ", _data[index]);
    }
    printf("\n");
}

void RAM::InitializeNintendoLogo() {
    _data[0x0104] = 0xCE; _data[0x0105] = 0xED; _data[0x0106] = 0x66; _data[0x0107] = 0x66; _data[0x0108] = 0xCC; _data[0x0109] = 0x0D; _data[0x010A] = 0x00; _data[0x010B] = 0x0B; _data[0x010C] = 0x03; _data[0x010D] = 0x73; _data[0x010E] = 0x00; _data[0x010F] = 0x83; _data[0x0110] = 0x00; _data[0x0111] = 0x0C; _data[0x0112] = 0x00; _data[0x0113] = 0x0D;
    _data[0x0114] = 0x00; _data[0x0115] = 0x08; _data[0x0116] = 0x11; _data[0x0117] = 0x1F; _data[0x0118] = 0x88; _data[0x0119] = 0x89; _data[0x011A] = 0x00; _data[0x011B] = 0x0E; _data[0x011C] = 0xDC; _data[0x011D] = 0xCC; _data[0x011E] = 0x6E; _data[0x011F] = 0xE6; _data[0x0120] = 0xDD; _data[0x0121] = 0xDD; _data[0x0122] = 0xD9; _data[0x0123] = 0x99;
    _data[0x0124] = 0xBB; _data[0x0125] = 0xBB; _data[0x0126] = 0x67; _data[0x0127] = 0x63; _data[0x0128] = 0x6E; _data[0x0129] = 0x0E; _data[0x012A] = 0xEC; _data[0x012B] = 0xCC; _data[0x012C] = 0xDD; _data[0x012D] = 0xDC; _data[0x012E] = 0x99; _data[0x012F] = 0x9F; _data[0x0130] = 0xBB; _data[0x0131] = 0xB9; _data[0x0132] = 0x33; _data[0x0133] = 0x3E;

    _data[0xFF44] = 0x90;
}
