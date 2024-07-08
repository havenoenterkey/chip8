#include "chip8.h"


void chip8_initialize(chip8 *chip8_pointer)
{
    // Set display
    init_SDL(&chip8_pointer->chip8_screen);
    prepare_scene(&chip8_pointer->chip8_screen);

    // Set memory
    initialize_memory(&chip8_pointer->chip8_memory);
    load_font(&chip8_pointer->chip8_memory);
}


void load_rom(chip8 *chip8_pointer, const char *path)
{
    FILE *file = fopen(path, "rb");

    size_t i = RAM_PROG_START;
    while (fread(&chip8_pointer->chip8_memory.RAM[i], sizeof(uint8_t), 1, file))
    {
        i++;
    }
    fclose(file);
}


// Combines two bytes into one instruction
uint16_t fetch(chip8 *chip8_pointer)
{
    // Get first byte
    uint16_t instruction = chip8_pointer->chip8_memory.RAM[chip8_pointer->chip8_memory.PC++];
    
    // Shift it to the left
    instruction = instruction << 8u;

    // Add second byte
    instruction = instruction | chip8_pointer->chip8_memory.RAM[chip8_pointer->chip8_memory.PC++];

    return instruction;
}


// Decodes and executes instructions
void decode_execute(chip8 *chip8_pointer)
{
    // Get all needed data
    uint16_t instruction = fetch(chip8_pointer);
    uint8_t X = (instruction & 0xF00) >> 8u;
    uint8_t Y = (instruction & 0xF0) >> 4u;
    uint16_t N = (instruction & 0xF);
    uint16_t NN = (instruction & 0xFF);
    uint16_t NNN = (instruction & 0xFFF);

    // Execute
    switch (instruction & 0xF000)
    {
        case 0x0000:
            switch (instruction)
            {
                case 0x00E0:
                    instruction_00E0(chip8_pointer);
                    break;
            }
            break;

        case 0x1000:
            instruction_1NNN(chip8_pointer, NNN);
            break;

        case 0x6000:
            instruction_6XNN(chip8_pointer, X, NN);
            break;

        case 0x7000:
            instruction_7XNN(chip8_pointer, X, NN);
            break;

        case 0xA000:
            instruction_ANNN(chip8_pointer, NNN);
            break;

        case 0xD000:
            instruction_DXYN(chip8_pointer, X, Y, N);
            break;
    }
}


// Instruction that clears screen
void instruction_00E0(chip8 *chip8_pointer)
{
    clear_scene(&chip8_pointer->chip8_screen);
}


// Instruction that sets pointer counter to NNN
void instruction_1NNN(chip8 *chip8_pointer, uint16_t NNN)
{
    chip8_pointer->chip8_memory.PC = NNN;
}


// Instruction that sets register VX to the value of NN
void instruction_6XNN(chip8 *chip8_pointer, uint8_t X, uint16_t NN)
{
    chip8_pointer->chip8_memory.V[X] = NN;
}


// Instruction that adds the value of NN to VX
void instruction_7XNN(chip8 *chip8_pointer, uint8_t X, uint16_t NN)
{
    chip8_pointer->chip8_memory.V[X] += NN;
}

// Sets the index register I to the value of NNN
void instruction_ANNN(chip8 *chip8_pointer, uint16_t NNN)
{
    chip8_pointer->chip8_memory.I = NNN;
}

//Instructions that draws
void instruction_DXYN(chip8 *chip8_pointer, uint8_t X, uint8_t Y, uint16_t N)
{
    // Fetch X and Y coordinates
    X = chip8_pointer->chip8_memory.V[X] % SCREEN_WIDTH;
    Y = chip8_pointer->chip8_memory.V[Y] % SCREEN_HEIGHT;

    // Set VF to 0
    chip8_pointer->chip8_memory.V[0xF] = 0;

    for (size_t i = 0; i < N; i++)
    {
        // Get sprite data
        uint8_t sprite_data = chip8_pointer->chip8_memory.RAM[chip8_pointer->chip8_memory.I + i];

        // For each of 8 pixels in this sprite row
        for (size_t j = 0; j < 8; j++)
        {
            // If reach right edge of the screen stop
            if (X + j >= SCREEN_WIDTH)
            {
                break;
            }

            // Get pixel
            uint8_t pixel = sprite_data & 0x80;
            sprite_data <<= 1u;

            // If pixel in the sprite row is on and on the screen is on, then turn off and set VF to 1            
            if (pixel && chip8_pointer->chip8_screen.PIXEL_GRID[X + j][Y + i])
            {
                chip8_pointer->chip8_screen.PIXEL_GRID[X + j][Y + i] = 0;
                draw_black_pixel(&chip8_pointer->chip8_screen, X + j, Y + i);
                chip8_pointer->chip8_memory.V[0xF] = 1;
                
            }
            // Or if curent pixel is on and on the screen is not on, draw it at X and Y
            else if (pixel && !chip8_pointer->chip8_screen.PIXEL_GRID[X + j][Y + i])
            {
                chip8_pointer->chip8_screen.PIXEL_GRID[X + j][Y + i] = 1;
                draw_white_pixel(&chip8_pointer->chip8_screen, X + j, Y + i);
            }
        }

        // Stop if reach bottom edge of the screen
        if (Y >= SCREEN_HEIGHT)
        {
            break;
        }
    }
}