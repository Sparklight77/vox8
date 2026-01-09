#include "video.h"

// Render framebuffer to SDL
void draw_display(CHIP8 *C8, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 64; x++) {
            int index = y * 64 + x;
            if (C8->DISPLAY[index]) {
                SDL_Rect pixel;
                pixel.x = x * SCALE;
                pixel.y = y * SCALE;
                pixel.w = SCALE;
                pixel.h = SCALE;
                SDL_RenderFillRect(renderer, &pixel);
            }
        }
    }
    SDL_RenderPresent(renderer);
}