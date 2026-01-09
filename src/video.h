#ifndef VIDEO_H
#define VIDEO_H

#include "chip8.h"
#include <SDL2/SDL.h>

#define SCALE 10
#define WINDOW_W (64 * SCALE)
#define WINDOW_H (32 * SCALE)

void draw_display(CHIP8 *C8, SDL_Renderer *renderer);

#endif