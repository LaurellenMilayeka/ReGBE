#include <iostream>

#include <RAM/RAM.h>
#include <CPU/CPU.h>
#include <Display/Display.h>

#include <SDL2/SDL.h>

int main(int, char**) {
    SDL_Event events;
    bool running = true;
    double frameElapsedInSec = 0.00;
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

        while (CPU::overallClock < CYCLES_PER_FRAME) {
            if (CPU::pc < 0x0100) {
                CPU::ReadNextInstruction(RAM::At(CPU::pc++));
                CPU::overallClock += CPU::ticks;
            }
            else {
                CPU::ticks = 4;
                CPU::overallClock += 4;
            }
        }
        CPU::overallClock %= CLOCK_SPEED;

        frameEnd = SDL_GetPerformanceCounter();
        frameElapsedInSec = (double)(frameEnd - frameStart) / (double)SDL_GetPerformanceFrequency();
        while (frameElapsedInSec < TIME_PER_FRAME)
        {
            frameEnd = SDL_GetPerformanceCounter();
            frameElapsedInSec = (double)(frameEnd - frameStart) / (double)SDL_GetPerformanceFrequency();
        }
    }
    CPU::PrintRegisters();
    return 0;
}
