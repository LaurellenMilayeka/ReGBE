#include <iostream>

#include <RAM/RAM.h>
#include <CPU/CPU.h>
#include <Display/Display.h>

#include <SDL2/SDL.h>

int main(int, char**) {
    SDL_Event events;
    bool running = true;

    std::cout << "Hello, World!" << std::endl;
    RAM::LoadBIOS("../ROMS/DMG_ROM.bin");
    RAM::LoadROM("../ROMS/Tetris.gb");

    Display::Initialize();
    while (running) {
        Display::Update();
        while (SDL_PollEvent(&events))
            switch (events.type)
            {
                case SDL_QUIT:
                    running = false;
            }
        if (CPU::pc < 0x0100)
            CPU::ReadNextInstruction(RAM::At(CPU::pc++));
        else
            CPU::ticks = 4;
    }
    CPU::PrintRegisters();
    return 0;
}
