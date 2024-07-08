#include <stdio.h>
#include <stdint.h>
#include "SDL2/SDL.h"






// Memory
uint8_t RAM[RAM_CAPACITY];

// Display
uint8_t PIXEL_GRID[SCREEN_WIDTH][SCREEN_HEIGHT];
SDL_Window *WINDOW = NULL;
SDL_Renderer *RENDERER = NULL;

// Program counter
uint16_t PC = RAM_PROG_START;

// Index register | points to current location in memory
uint16_t I = 0;

// Stack
uint16_t STACK[STACK_SIZE];

// Timers
uint8_t DELAY_TIMER = 0, SOUND_TIMER = 0;

// Variable registers 0 through F
uint8_t V[V_SIZE];


// Emulator functions
uint16_t fetch(void);
void decode_execute(void);
void load_rom(const char *name);


// SDL functions
void initialize_window(void);
void draw_pixel_white(int width, int height);
void draw_pixel_black(int width, int height);


// Debugging functions
void print_ram(uint8_t ram[]);
void random_debug(void);


int main(int argc, char *argv[])
{
    // Set memory to 0
    for (size_t i = RAM_START; i <= RAM_END; ++i)
        RAM[i] = 0x00;

    // Set stack to 0
    for (size_t i = 0; i < STACK_SIZE; ++i)
        STACK[i] = 0;

    // Load font
    FILE *font_data = fopen("font.txt", "r");
    for (size_t i = RAM_START; fscanf(font_data, "%hhx", &RAM[i]) != EOF; ++i);

    // Initialize display
    initialize_window();
    for (size_t i = 0; i < SCREEN_WIDTH; ++i)
    {
        for (size_t j = 0; j < SCREEN_HEIGHT; ++j)
        {
            PIXEL_GRID[i][j] = 0;
        }
    }

    //random_debug();

    // Reading to memory
    if (argc == 2)
    {
        load_rom(argv[1]);
        print_ram(RAM);

        for (size_t i = 0; i < 50; ++i)
            decode_execute();

        getchar();
    }
    else
    {
        printf("Fail\n");
        return 1;
    }

    return 0;
}


uint16_t fetch(void)
{
    // Combine two 8 bit values into one 16 bit instruction
    // Increase PC by two
    uint16_t instruction = RAM[PC++] << 8;
    instruction = instruction | RAM[PC++];

    return instruction;
}


void decode_execute(void)
{
    // Get instruction
    uint16_t instruction = fetch();
    uint16_t nibbles[4];

    // Get individual nibbles
    nibbles[0] = instruction & 0xF000;
    nibbles[1] = instruction & 0x0F00;
    nibbles[2] = instruction & 0x00F0;
    nibbles[3] = instruction & 0x000F;

    // Decode
    uint16_t X = nibbles[1];
    uint16_t Y = nibbles[2];
    uint16_t N = nibbles[3];
    uint16_t NN = nibbles[2] | nibbles[3];
    uint16_t NNN = nibbles[1] | nibbles[2] | nibbles[3];


    printf("Instruction: %.4x\n", instruction);
    printf("X: %.4x\n", X);
    printf("Y: %.4x\n", Y);
    printf("N %.4x\n", N);
    printf("NN: %.4x\n", NN);
    printf("NNN %.4x\n", NNN);
    printf("\n");

    // Execute
    switch (nibbles[0])
    {
        case 0x0000:
            switch (instruction)
            {
                case 0x00E0:
                    SDL_SetRenderDrawColor(RENDERER, 0, 0, 0, 255);
                    SDL_RenderClear(RENDERER);
                    SDL_RenderPresent(RENDERER);
                    break;
            }
            break;

        case 0x1000:
            PC = NNN;
            break;

        case 0x6000:
            V[X] = NN;
            break;

        case 0x7000:
            V[X] += NN;
            break;

        case 0xA000:
            I = NNN;
            break;

        case 0xD000:
            uint8_t draw_x = V[X] % 64;
            uint8_t draw_y = V[Y] % 32;

            printf("draw x: %d\n", draw_x);
            printf("draw y: %d\n", draw_y);

            V[0xF] = 0;

            for (size_t i = 0; i < N; ++i)
            {
                uint8_t sprite_data = RAM[I + i];

                for (size_t j = 0; j < 8; j++)
                {

                    if (draw_x > SCREEN_WIDTH)
                        break;

                    // Get pixel
                    uint8_t pixel = (sprite_data >> (8 - j)) << 7;
                    //printf("%x", pixel);
                    if (pixel && PIXEL_GRID[draw_x][draw_y])
                    {
                        PIXEL_GRID[draw_x][draw_y] = 0;
                        draw_pixel_black(draw_x, draw_y);
                        V[0xF] = 1;
                    }
                    else if (pixel && !PIXEL_GRID[draw_x][draw_y])
                    {                        
                        PIXEL_GRID[draw_x][draw_y] = 1;
                        draw_pixel_white(draw_x, draw_y);
                    }

                    ++draw_x;
                }

                ++draw_y;
                if (draw_y > SCREEN_HEIGHT)
                    break;
            }

            break;
    }
}


void load_rom(const char *name)
{
    FILE *file = fopen(name, "rb");

    size_t i = RAM_PROG_START;
    while (fread(&RAM[i], sizeof(char), 1, file))
        ++i;
    fclose(file);
}


void initialize_window(void)
{

    WINDOW = SDL_CreateWindow(
        "CHIP8",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        0
    );

    RENDERER = SDL_CreateRenderer(WINDOW, -1, 0);
    
    SDL_SetRenderDrawColor(RENDERER, 0, 0, 0, 255);
    SDL_RenderClear(RENDERER);
    SDL_RenderPresent(RENDERER);
}


void draw_pixel_white(int width, int height)
{
    SDL_SetRenderDrawColor(RENDERER, 255, 255, 255, 255);
    SDL_RenderDrawPoint(RENDERER, width, height);
    SDL_RenderPresent(RENDERER);
}


void draw_pixel_black(int width, int height)
{
    SDL_SetRenderDrawColor(RENDERER, 0, 0, 0, 255);
    SDL_RenderDrawPoint(RENDERER, width, height);
    SDL_RenderPresent(RENDERER);
}


void print_ram(uint8_t ram[])
{
    for (size_t i = RAM_START; i <= RAM_END; ++i)
    {
        printf("%x ", ram[i]);
        
        if ((i + 1) % 5 == 0)
        {
            printf("\n");
        }
    }
    printf("\n");
}


void random_debug(void)
{
    PC = RAM_START;
    decode_execute();
    decode_execute();
    decode_execute();
    decode_execute();
}