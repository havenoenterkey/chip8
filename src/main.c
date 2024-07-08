#include <stdio.h>
#include "chip8.h"

int main(int argc, char *argv[])
{
    // Set display
    chip8 main_chip8;
    chip8_initialize(&main_chip8);

    for (int i = 1; i < 30; i++)
    {
        draw_white_pixel(&main_chip8.chip8_screen, i, i);
    }
    getchar();

    return 0;
}