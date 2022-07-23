#include <iostream>

#include <RAM/RAM.h>
#include <CPU/CPU.h>
#include <Display/Display.h>

#include <SDL2/SDL.h>

int main(int, char**) {
    SDL_Event events;
    bool running = true;
    std::uint64_t frameStart = 0, frameEnd = 0;

    RAM::LoadBIOS("../ROMS/DMG_ROM.bin");
    RAM::LoadROM("../ROMS/Tetris.gb");

    Display::Initialize();
    while (running) {
        frameStart = SDL_GetPerformanceCounter();

        Display::Update();
        while (SDL_PollEvent(&events))
            switch (events.type)
            {
                case SDL_QUIT:
                    running = false;
            }

        //while (CPU::overallClock < CLOCK_SPEED) {
        CPU::PrintRegisters();
        CPU::ReadNextInstruction(RAM::At(CPU::pc.reg++));
        CPU::overallClock += CPU::ticks;
        CPU::PrintRegisters();
        //}
        CPU::overallClock %= CLOCK_SPEED;

        /*while (true) {
            frameEnd = SDL_GetPerformanceCounter();
            double frameElapsedInSec = (double)(frameEnd - frameStart) / (double)SDL_GetPerformanceFrequency();
            if (frameElapsedInSec >= TIME_PER_FRAME) {
                break;
            }
        }*/
    }
    RAM::Dump(0x8000, 0x9FFF);
    CPU::PrintRegisters();
    return 0;
}
