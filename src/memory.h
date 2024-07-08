#ifndef MEMORY
#define MEMORY

#include <stdint.h>
#include <stdio.h>

#define RAM_START 0x000
#define RAM_END 0xFFF
#define RAM_PROG_START 0x200
#define RAM_CAPACITY 4096u
#define STACK_SIZE 16u
#define V_SIZE 16u

typedef struct memory
{
    uint8_t RAM[RAM_CAPACITY];
    uint8_t STACK[STACK_SIZE];
    uint8_t V[V_SIZE];
    uint16_t PC;
    uint16_t I;
    uint8_t DELAY_TIMER;
    uint16_t SOUND_TIMER;
} memory;

void initialize_memory(memory *memory_pointer);
void read_ram(memory *memory_pointer);
void load_font(memory *memory_pointer);

#endif