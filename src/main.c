#include <stdio.h>
#include <SDL2/SDL.h>
#include "chip8.h"
#include "video.h"
#include "audio.h"
#include "input.h"

int main(int argc, char **argv) {
    CHIP8 system;
    systemInit(&system, fontset, sizeof(fontset));

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <rom.ch8>\n", argv[0]);
        return 1;
    }
    loadROM(&system, argv[1]);

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("CHIP-8",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_W, WINDOW_H,
        SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        fprintf(stderr, "Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Open simple audio device, logic written with the help of ChatGPT
    SDL_AudioSpec want, have;
    SDL_memset(&want, 0, sizeof(want));
    want.freq = 48000;
    want.format = AUDIO_S16SYS;
    want.channels = 1;
    want.samples = 1024;
    want.callback = audio_callback;
    want.userdata = NULL;

    SDL_AudioDeviceID dev = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);
    if (dev == 0) {
        fprintf(stderr, "Failed to open audio: %s\n", SDL_GetError());
    } else {
        set_audio_system(&system);
        SDL_PauseAudioDevice(dev, 0); // start audio
    }

    bool running = true;
    SDL_Event event;

    uint32_t last_timer_tick = SDL_GetTicks();
    
    const int cycles_per_loop = 6;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
                bool pressed = (event.type == SDL_KEYDOWN);
                int mapped = map_sdl_key_to_chip8(event.key.keysym.sym);
                if (mapped >= 0) {
                    system.KEY[mapped] = pressed;
                } else if (event.key.keysym.sym == SDLK_ESCAPE && event.type == SDL_KEYDOWN) {
                    running = false;
                }
            }
        }

       
        for (int i = 0; i < cycles_per_loop; i++) {
            cpu_cycle(&system);
        }

        // Timers at 60Hz
        uint32_t now = SDL_GetTicks();
        if (now - last_timer_tick >= 16) { // ~16ms
            update_timers(&system);
            last_timer_tick = now;
        }

        if (system.FLAG_DRAW) {
            draw_display(&system, renderer);
            system.FLAG_DRAW = false;
        }

        SDL_Delay(1);
    }

    if (dev) SDL_CloseAudioDevice(dev);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}