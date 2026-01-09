#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t V[16];
    uint16_t I;
    uint16_t PC;
    uint8_t SP;

    uint8_t MEMORY[4096];
    uint16_t STACK[16];

    bool DISPLAY[64 * 32];
    bool FLAG_DRAW;

    uint8_t DELAY_TIMER;
    uint8_t SOUND_TIMER;

    bool KEY[16];
} CHIP8;

void systemInit(CHIP8 *C8, const uint8_t *fontset, size_t fontset_size);
void loadROM(CHIP8 *C8, const char *filename);
void cpu_cycle(CHIP8 *C8);
void update_timers(CHIP8 *C8);

extern uint8_t fontset[80];

#endif