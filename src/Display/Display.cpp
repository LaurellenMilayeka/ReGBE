//
// Created by ML52298126 on 19/07/2022.
//

#include "Display.h"
#include "CPU/CPU.h"

#include <RAM/RAM.h>
#include <iostream>

std::uint8_t Display::rmask = 0x00;
std::uint8_t Display::gmask = 0x00;
std::uint8_t Display::bmask = 0x00;
std::uint8_t Display::amask = 0x00;

SDL_Window* Display::Window = nullptr;
SDL_Surface* Display::SurfForPitch = nullptr;
SDL_Renderer* Display::Renderer = nullptr;
SDL_Texture* Display::Texture = nullptr;
std::uint32_t Display::Pixels[160*144] = {};
std::uint16_t Display::GPUClock = 0x00;
Display::GPUState Display::GPUMode = Display::GPUState::SCANLINE_OAM;

void Display::Initialize() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
        std::cerr << "Could not initialize graphics" << std::endl;
    else
        std::cout << "SDL Initialized correctly" << std::endl;

    if (SDL_CreateWindowAndRenderer(160 * 4, 144 * 4, 0, &Window, &Renderer) != 0)
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
    for (unsigned int Pixel : Pixels)
        Pixel = SDL_MapRGBA(SurfForPitch->format, 0x00, 0x00, 0x00, 0x00);
}

void Display::Update() {
    GPUClock += CPU::ticks;
    if ((RAM::At(0xFF40) & 0x80) == 0x80) {
        switch (GPUMode) {
            case Display::GPUState::SCANLINE_OAM:
                if (GPUClock >= 80) {
                    GPUClock = 0;
                    GPUMode  = Display::GPUState::SCANLINE_VRAM;
                }
                break;
            case Display::GPUState::SCANLINE_VRAM:
                if (GPUClock >= 172) {
                    GPUClock = 0;
                    GPUMode  = Display::GPUState::HORIZONTAL_BLANK;
                }
                break;
            case Display::GPUState::HORIZONTAL_BLANK:
                if (GPUClock >= 204) {
                    UpdateLine();
                    RAM::SetAt(0xFF44, RAM::At(0xFF44) + 0x01);
                    GPUClock = 0;
                    if (RAM::At(0xFF44) == 144) {
                        GPUMode  = Display::GPUState::VERTICAL_BLANK;
                    }
                    else {
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
                        SDL_RenderPresent(Renderer);
                        GPUMode = Display::GPUState::SCANLINE_OAM;
                        RAM::SetAt(0xFF44, 0x00);
                    }
                }
                break;
        }
    }
}

void Display::UpdateLine() {
    std::uint8_t SCX = RAM::At(0xFF43);
    std::uint8_t SCY = RAM::At(0xFF42);
    std::uint8_t Y   = SCY + RAM::At(0xFF44);
    std::uint16_t tileRow = (Y / 8);

    std::uint16_t bgTileMapAddress = (RAM::At(0xFF40) & 0x08) == 0x08 ? 0x9C00 : 0x9800;
    std::uint16_t bgWindowTileData = (RAM::At(0xFF40) & 0x10) == 0x10 ? 0x8000 : 0x8800;
    std::uint16_t windowTileMapDisplaySelect = (RAM::At(0xFF40) & 0x40) == 0x40 ? 0x9C00 : 0x9800;
    bool bgWindowPriority = (RAM::At(0xFF40) & 0x01) == 0x01;
    bool spriteDisplayEnable = (RAM::At(0xFF40) & 0x02) == 0x02;

    for (unsigned int pixel = 0; pixel < 160; pixel++) {
        std::uint8_t X = SCX + pixel;
        std::uint16_t tileColumn = (X / 8);
        std::uint16_t tileAddress = (bgTileMapAddress + tileRow * 32 + tileColumn);
        std::uint8_t tileNumber = RAM::At(tileAddress);
        std::uint16_t tile = bgWindowTileData + (tileNumber * 16);
        std::uint8_t lno = Y % 8;
        std::uint8_t data1 = RAM::At(tile + lno * 2);
        std::uint8_t data2 = RAM::At(tile + lno * 2 + 1);
        std::uint8_t req_bit = (7 - (X % 8));
        std::uint8_t bit1 = (data1 >> req_bit) & 1;
        std::uint8_t bit2 = (data2 >> req_bit) & 1;
        std::uint8_t colorID = (bit2 << 1) | (bit1);
        std::uint8_t finalColor = GetColour(colorID, 0xFF47);
        std::uint8_t red, green, blue;
        switch (finalColor) {
            case 0:
                red = 255; green = 255; blue = 255;
                break;
            case 1:
                red = 192; green = 192; blue = 192;
                break;
            case 2:
                red = 96; green = 96; blue = 96;
                break;
            case 3:
            default:
                red = 0; green = 0; blue = 0;
                break;
        }
        Pixels[(RAM::At(0xFF44) * 160) + pixel] = SDL_MapRGB(SurfForPitch->format, red, green, blue);
    }
}

std::uint8_t Display::GetColour(std::uint8_t data, std::uint16_t address) {
    std::uint8_t palette = RAM::At(address);
    int hi = 0, lo = 0;

    switch (data)
    {
        case 0:
            hi = 1;
            lo = 0;
            break;
        case 1:
            hi = 3;
            lo = 2;
            break;
        case 2:
            hi = 5;
            lo = 4;
            break;
        case 3:
        default:
            hi = 7;
            lo = 6;
            break;
    }

    int ret_colour = 0;
    ret_colour = ((palette >> hi) & 1) << 1;
    ret_colour |= ((palette >> lo) & 1);

    return (ret_colour);
}