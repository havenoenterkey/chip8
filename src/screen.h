#ifndef SDL_MAIN_HANDLED
#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

#define SCREEN_WIDTH 64u
#define SCREEN_HEIGHT 32u

typedef struct screen
{
    SDL_Renderer *renderer;
    SDL_Window *window;
    bool PIXEL_GRID[SCREEN_WIDTH][SCREEN_HEIGHT];
}
screen;

void init_SDL(screen *current_screen);
void prepare_scene(screen *current_screen);
void clear_scene(screen *current_screen);
void draw_white_pixel(screen *current_screen, unsigned x, unsigned y);
void draw_black_pixel(screen *current_screen, unsigned x, unsigned y);
void display_pixel_grid(screen *current_screen);

#endif