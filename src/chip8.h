#ifndef CHIP8
#define CHIP8

#include "memory.h"
#include "screen.h"

typedef struct chip8
{
    memory chip8_memory;
    screen chip8_screen;
} chip8;

void chip8_initialize(chip8 *chip8_pointer);
void load_rom(chip8 *chip8_pointer, const char *path);

uint16_t fetch(chip8 *chip8_pointer);
void decode_execute(chip8 *chip8_pointer);

void instruction_00E0(chip8 *chip8_pointer);
void instruction_1NNN(chip8 *chip8_pointer, uint16_t NNN);
void instruction_6XNN(chip8 *chip8_pointer, uint8_t X, uint16_t NN);
void instruction_7XNN(chip8 *chip8_pointer, uint8_t X, uint16_t NN);
void instruction_ANNN(chip8 *chip8_pointer, uint16_t NNN);
void instruction_DXYN(chip8 *chip8_pointer, uint8_t X, uint8_t Y, uint16_t N);

#endif