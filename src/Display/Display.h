//
// Created by ML52298126 on 19/07/2022.
//

#ifndef GBE_DISPLAY_H
#define GBE_DISPLAY_H

#include <SDL2/SDL.h>
#include <cstdint>

class Display {

private:

    enum class GPUState {
        HORIZONTAL_BLANK = 0,
        VERTICAL_BLANK = 1,
        SCANLINE_OAM = 2,
        SCANLINE_VRAM = 3
    };

    static std::uint8_t rmask, gmask, bmask, amask;

    static SDL_Window* Window;
    static SDL_Surface* SurfForPitch;
    static SDL_Renderer* Renderer;
    static SDL_Texture* Texture;

    static std::uint8_t Pixels[160*144];

    static std::uint16_t GPUClock;
    static GPUState GPUMode;

public:
    static void Initialize();

    static void Update();
};


#endif //GBE_DISPLAY_H
