#include "chip8.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

uint8_t fontset[80] = {
    0xF0,0x90,0x90,0x90,0xF0, // 0
    0x20,0x60,0x20,0x20,0x70, // 1
    0xF0,0x10,0xF0,0x80,0xF0, // 2
    0xF0,0x10,0xF0,0x10,0xF0, // 3
    0x90,0x90,0xF0,0x10,0x10, // 4
    0xF0,0x80,0xF0,0x10,0xF0, // 5
    0xF0,0x80,0xF0,0x90,0xF0, // 6
    0xF0,0x10,0x20,0x40,0x40, // 7
    0xF0,0x90,0xF0,0x90,0xF0, // 8
    0xF0,0x90,0xF0,0x10,0xF0, // 9
    0xF0,0x90,0xF0,0x90,0x90, // A
    0xE0,0x90,0xE0,0x90,0xE0, // B
    0xF0,0x80,0x80,0x80,0xF0, // C
    0xE0,0x90,0x90,0x90,0xE0, // D
    0xF0,0x80,0xF0,0x80,0xF0, // E
    0xF0,0x80,0xF0,0x80,0x80  // F
};

void systemInit(CHIP8 *C8, const uint8_t *fontset, size_t fontset_size) {
    memset(C8, 0, sizeof(CHIP8));

    C8->PC = 0x200;
    C8->SP = 0;
    C8->I = 0;

    memset(C8->DISPLAY, 0, sizeof(C8->DISPLAY));
    C8->FLAG_DRAW = false;

    // Load fontset
    if (fontset && fontset_size <= (0x200 - 0x50)) {
        memcpy(&C8->MEMORY[0x50], fontset, fontset_size);
    }

    // seed RNG
    srand((unsigned)time(NULL));
}

void loadROM(CHIP8 *C8, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Failed to open ROM: %s\n", filename);
        return;
    }

    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    rewind(file);

    if (fsize <= 0) {
        fprintf(stderr, "ROM is empty or unreadable\n");
        fclose(file);
        return;
    }

    // overflow lock
    long max_size = sizeof(C8->MEMORY) - 0x200;
    if (fsize > max_size) fsize = max_size;

    size_t read = fread(&C8->MEMORY[0x200], 1, (size_t)fsize, file);
    if (read == 0) {
        fprintf(stderr, "Failed to read ROM data\n");
    }
    fclose(file);
}

void update_timers(CHIP8 *C8) {
    if (C8->DELAY_TIMER > 0) C8->DELAY_TIMER--;
    if (C8->SOUND_TIMER > 0) C8->SOUND_TIMER--;
}