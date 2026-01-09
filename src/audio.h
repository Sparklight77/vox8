#ifndef AUDIO_H
#define AUDIO_H

#include "chip8.h"
#include <SDL2/SDL.h>

void audio_callback(void *userdata, Uint8 *stream, int len);
void set_audio_system(CHIP8 *C8);

#endif