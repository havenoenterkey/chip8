#include "memory.h"


void initialize_memory(memory *memory_pointer)
{
    // Set RAM to 0
    for (size_t i = RAM_START; i < RAM_END; i++)
        memory_pointer->RAM[i] = 0x00;

    // Set stack to 0
    for (size_t i = 0; i < STACK_SIZE; ++i)
        memory_pointer->STACK[i] = 0;

    // Set v to 0
    for (size_t i = 0; i < V_SIZE; ++i)
        memory_pointer->V[i] = 0;

    // Set to 0 everything else
    memory_pointer->PC = RAM_PROG_START;
    memory_pointer->I = 0;
    memory_pointer->SOUND_TIMER = 0;
    memory_pointer->DELAY_TIMER = 0;
}


void read_ram(memory *memory_pointer)
{
    for (size_t i = RAM_START; i < RAM_END; i++)
    {
        if (i % 10 == 0 && i > 0)
            printf("\n");
        printf("%hhx ", memory_pointer->RAM[i]);
    }
}


void load_font(memory *memory_pointer)
{
    // Load font
    FILE *font_data = fopen("../font.txt", "r");
    for (size_t i = RAM_START; fscanf(font_data, "%hhx", &memory_pointer->RAM[i]) != EOF; i++);
}