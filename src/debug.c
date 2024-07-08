#include <stdio.h>
#include "chip8.h"

int main(int argc, char *argv[])
{
    chip8 main_chip8;
    chip8_initialize(&main_chip8);
    load_rom(&main_chip8, "../ibm.ch8");

    for (size_t i = 0; i < 50; i++)
        decode_execute(&main_chip8);
    getchar();


    return 0;
}