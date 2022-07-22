//
// Created by ML52298126 on 19/07/2022.
//

#include "Display.h"
#include "CPU/CPU.h"

#include <RAM/RAM.h>
#include <iostream>
#include <cstring>

std::uint8_t Display::rmask = 0x00;
std::uint8_t Display::gmask = 0x00;
std::uint8_t Display::bmask = 0x00;
std::uint8_t Display::amask = 0x00;

SDL_Window* Display::Window = nullptr;
SDL_Surface* Display::SurfForPitch = nullptr;
SDL_Renderer* Display::Renderer = nullptr;
SDL_Texture* Display::Texture = nullptr;
std::uint8_t Display::Pixels[160*144] = {};
std::uint16_t Display::GPUClock = 0x00;
Display::GPUState Display::GPUMode = Display::GPUState::SCANLINE_OAM;

void Display::Initialize() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
        std::cerr << "Could not initalize graphics" << std::endl;
    else
        std::cout << "SDL Initialized correctly" << std::endl;

    if (SDL_CreateWindowAndRenderer(160, 144, 0, &Window, &Renderer) != 0)
        std::cerr << "Could not create Window and Renderer" << std::endl;
    else
        std::cout << "Window and Renderer successfully created" << std::endl;

    SurfForPitch = SDL_CreateRGBSurface(0, 160, 144, 32, rmask, gmask, bmask, amask);
    if (SurfForPitch == nullptr)
        std::cerr << "Could not create temporary surface" << std::endl;
    else
        std::cout << "Temporary surface created and pitch retrieved" << std::endl;

    Texture = SDL_CreateTexture(Renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, 160, 144);
    if (Texture == nullptr)
        std::cerr << "Could not create texture" << std::endl;
    else
        std::cout << "Drawing canvas created successfully" << std::endl;

    SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
    for (unsigned int index = 0; index < 160*144; index++)
        Pixels[index] = SDL_MapRGBA(SurfForPitch->format, 0x00, 0x00, 0x00, 0x00);
}

void Display::Update() {
    std::uint16_t bgTileMapAddress = (RAM::At(0xFF40) & 0x08) == 0x08 ? 0x9C00 : 0x9800;
    std::uint16_t bgWindowTileData = (RAM::At(0xFF40) & 0x10) == 0x10 ? 0x8000 : 0x8800;
    std::uint16_t windowTileMapDisplaySelect = (RAM::At(0xFF40) & 0x40) == 0x40 ? 0x9C00 : 0x9800;
    bool bgWindowPriority = (RAM::At(0xFF40) & 0x01) == 0x01 ? true : false;
    bool spriteDisplayEnable = (RAM::At(0xFF40) & 0x02) == 0x02 ? true : false;

    GPUClock += CPU::ticks;

    SDL_RenderClear(Renderer);
    if ((RAM::At(0xFF40) & 0x80) == 0x80) {
        switch (GPUMode) {
            case Display::GPUState::SCANLINE_OAM:
                if (GPUClock >= 80) {
                    GPUClock = 0;
                    std::cout << "ENTERING SCANLINE VRAM" << std::endl;
                    GPUMode  = Display::GPUState::SCANLINE_VRAM;
                }
                break;
            case Display::GPUState::SCANLINE_VRAM:
                if (GPUClock >= 172) {
                    GPUClock = 0;
                    std::cout << "ENTERING HORIZONTAL BLANK" << std::endl;
                    GPUMode  = Display::GPUState::HORIZONTAL_BLANK;
                }
                break;
            case Display::GPUState::HORIZONTAL_BLANK:
                if (GPUClock >= 204) {
                    RAM::SetAt(0xFF44, RAM::At(0xFF44) + 0x01);
                    GPUClock = 0;
                    if (RAM::At(0xFF44) == 143) {
                        std::cout << "ENTERING VERTICAL BLANK" << std::endl;
                        GPUMode  = Display::GPUState::VERTICAL_BLANK;
                    }
                    else {
                        std::cout << "ENTERING SCANLINE OAM" << std::endl;
                        GPUMode  = Display::GPUState::SCANLINE_OAM;
                    }
                }
                break;
            case Display::GPUState::VERTICAL_BLANK:
                if (GPUClock >= 456) {
                    RAM::SetAt(0xFF44, RAM::At(0xFF44) + 0x01);
                    GPUClock = 0;
                    if (RAM::At(0xFF44) > 153) {
                        SDL_UpdateTexture(Texture, nullptr, Pixels, SurfForPitch->pitch);
                        SDL_RenderCopy(Renderer, Texture, nullptr, nullptr);
                        std::cout << "ENTERING SCANLINE OAM" << std::endl;
                        GPUMode = Display::GPUState::SCANLINE_OAM;
                        RAM::SetAt(0xFF44, 0x00);
                    }
                }
                break;
        }
    }
    SDL_RenderPresent(Renderer);
}
