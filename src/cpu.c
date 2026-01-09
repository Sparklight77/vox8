#include "chip8.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void cpu_cycle(CHIP8 *C8) {
    
    if (C8->PC >= sizeof(C8->MEMORY) - 1) {
        fprintf(stderr, "PC out of bounds: 0x%04X\n", C8->PC);
        return;
    }

    uint16_t opcode =
        (C8->MEMORY[C8->PC] << 8) |
         C8->MEMORY[C8->PC + 1];

    C8->PC += 2;

    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode & 0x00FF) {
                case 0xE0: 
                    memset(C8->DISPLAY, 0, sizeof C8->DISPLAY);
                    C8->FLAG_DRAW = true;
                    break;
                case 0xEE: 
                    if (C8->SP == 0) {
                        fprintf(stderr, "Stack underflow on RET\n");
                        break;
                    }
                    C8->SP--;
                    C8->PC = C8->STACK[C8->SP];
                    break;
                default:
                   
                    break;
            }
            break;

        case 0x1000: { 
            uint16_t addr = opcode & 0x0FFF;
            C8->PC = addr;
            break;
        }

        case 0x2000: {
            uint16_t addr = opcode & 0x0FFF;
            C8->STACK[C8->SP++] = C8->PC;
            C8->PC = addr;
            break;
        }

        case 0x3000: { // SE Vx, byte
            uint8_t X = (opcode & 0x0F00) >> 8;
            uint8_t NN = opcode & 0x00FF;
            if (C8->V[X] == NN) C8->PC += 2;
            break;
        }

        case 0x4000: { // SNE Vx, byte
            uint8_t X = (opcode & 0x0F00) >> 8;
            uint8_t NN = opcode & 0x00FF;
            if (C8->V[X] != NN) C8->PC += 2;
            break;
        }

        case 0x5000: { // SE Vx, Vy
            uint8_t X = (opcode & 0x0F00) >> 8;
            uint8_t Y = (opcode & 0x00F0) >> 4;
            if (C8->V[X] == C8->V[Y]) C8->PC += 2;
            break;
        }

        case 0x6000: { // LD Vx, byte
            uint8_t X = (opcode & 0x0F00) >> 8;
            C8->V[X] = opcode & 0x00FF;
            break;
        }

        case 0x7000: { // ADD Vx, byte
            uint8_t X = (opcode & 0x0F00) >> 8;
            C8->V[X] += opcode & 0x00FF;
            break;
        }

        case 0x8000: {
            uint8_t X = (opcode & 0x0F00) >> 8;
            uint8_t Y = (opcode & 0x00F0) >> 4;
            switch (opcode & 0x000F) {
                case 0x0: C8->V[X] = C8->V[Y]; break;
                case 0x1: C8->V[X] |= C8->V[Y]; break;
                case 0x2: C8->V[X] &= C8->V[Y]; break;
                case 0x3: C8->V[X] ^= C8->V[Y]; break;
                case 0x4: {
                    uint16_t sum = C8->V[X] + C8->V[Y];
                    C8->V[0xF] = sum > 0xFF;
                    C8->V[X] = sum & 0xFF;
                    break;
                }
                case 0x5: {
                    C8->V[0xF] = C8->V[X] >= C8->V[Y];
                    C8->V[X] -= C8->V[Y];
                    break;
                }
                case 0x6: {
                    C8->V[0xF] = C8->V[X] & 0x1;
                    C8->V[X] >>= 1;
                    break;
                }
                case 0x7: {
                    C8->V[0xF] = C8->V[Y] >= C8->V[X];
                    C8->V[X] = C8->V[Y] - C8->V[X];
                    break;
                }
                case 0xE: {
                    C8->V[0xF] = (C8->V[X] & 0x80) >> 7;
                    C8->V[X] <<= 1;
                    break;
                }
                default: break;
            }
            break;
        }

        case 0x9000: { // SNE Vx, Vy
            uint8_t X = (opcode & 0x0F00) >> 8;
            uint8_t Y = (opcode & 0x00F0) >> 4;
            if (C8->V[X] != C8->V[Y]) C8->PC += 2;
            break;
        }

        case 0xA000: { // LD I, addr
            uint16_t NNN = opcode & 0x0FFF;
            C8->I = NNN;
            break;
        }

        case 0xB000: { // JP V0, addr
            uint16_t NNN = opcode & 0x0FFF;
            C8->PC = C8->V[0] + NNN;
            break;
        }

        case 0xC000: { // RND Vx, byte
            uint8_t X = (opcode & 0x0F00) >> 8;
            uint8_t NN = opcode & 0x00FF;
            uint8_t r = (uint8_t)(rand() & 0xFF);
            C8->V[X] = r & NN;
            break;
        }

        case 0xE000: {
            uint8_t X = (opcode & 0x0F00) >> 8;
            uint8_t key_value = C8->V[X];
            if (key_value > 0xF) break;

            switch (opcode & 0x00FF) {
                case 0x9E:
                    if (C8->KEY[key_value]) C8->PC += 2;
                    break;
                case 0xA1:
                    if (!C8->KEY[key_value]) C8->PC += 2;
                    break;
                default: break;
            }
            break;
        }

        case 0xD000: { // DRW Vx, Vy, nibble (probably)
            uint8_t xIndex = (opcode & 0x0F00) >> 8;
            uint8_t yIndex = (opcode & 0x00F0) >> 4;
            uint8_t X = C8->V[xIndex];
            uint8_t Y = C8->V[yIndex];
            uint8_t N = opcode & 0x000F;
            C8->V[0xF] = 0;

            for (int row = 0; row < N; row++) {
                uint8_t sprite = C8->MEMORY[C8->I + row];

                for (int col = 0; col < 8; col++) {
                    if (sprite & (0x80 >> col)) {
                        int px = (X + col) % 64;
                        int py = (Y + row) % 32;
                        int index = py * 64 + px;

                        if (C8->DISPLAY[index]) {
                            C8->V[0xF] = 1;
                        }
                        C8->DISPLAY[index] ^= 1;
                    }
                }
            }
            C8->FLAG_DRAW = true;
            break;
        }

        case 0xF000: {
            uint8_t X = (opcode & 0x0F00) >> 8;
            switch (opcode & 0x00FF) {
                case 0x07: C8->V[X] = C8->DELAY_TIMER; break;
                case 0x0A: { // LD Vx, K 
                    bool key_pressed = false;
                    for (int i = 0; i < 16; i++) {
                        if (C8->KEY[i]) {
                            C8->V[X] = i;
                            key_pressed = true;
                            break;
                        }
                    }
                    if (!key_pressed) C8->PC -= 2;
                    break;
                }
                case 0x15: C8->DELAY_TIMER = C8->V[X]; break;
                case 0x18: C8->SOUND_TIMER = C8->V[X]; break;
                case 0x1E: C8->I += C8->V[X]; break;
                case 0x29: C8->I = 0x50 + (C8->V[X] * 5); break;
                case 0x33: { // BCD
                    C8->MEMORY[C8->I]   = C8->V[X] / 100;
                    C8->MEMORY[C8->I+1] = (C8->V[X] / 10) % 10;
                    C8->MEMORY[C8->I+2] = C8->V[X] % 10;
                    break;
                }
                case 0x55: { // LD [I], V0..VX
                    for (int i = 0; i <= X; i++) {
                        C8->MEMORY[C8->I + i] = C8->V[i];
                    }
                    break;
                }
                case 0x65: { 
                    for (int i = 0; i <= X; i++) {
                        C8->V[i] = C8->MEMORY[C8->I + i];
                    }
                    break;
                }
                default: break;
            }
            break;
        }

        default:
            printf("Unknown opcode: 0x%04X\n", opcode);
            break;
    }
}