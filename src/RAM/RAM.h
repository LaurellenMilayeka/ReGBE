//
// Created by ML52298126 on 15/07/2022.
//

#ifndef GBE_RAM_H
#define GBE_RAM_H

#include <string>
#include <bitset>

class RAM {

private:

    static unsigned char _romEntryPoint[0x0100];
    static unsigned char _data[0xFFFF];

public:

    static void InitializeNintendoLogo();
    static void DisplayCRC();

    static bool LoadROM(std::string const& romPath);

    static unsigned char At(std::uint16_t index);
    static void SetAt(std::uint16_t index, std::uint8_t value);

    static void Dump(std::uint16_t addrStart, std::uint16_t addrEnd);

    static bool LoadBIOS(const std::string &biosPath);
};


#endif //GBE_RAM_H
