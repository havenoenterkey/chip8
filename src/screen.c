#include "screen.h"


void init_SDL(screen *current_screen)
{
    int renderer_flags = SDL_RENDERER_ACCELERATED;
    int window_flags = 0;

    // If couldn't initialize
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    // Initialize window
    current_screen->window = SDL_CreateWindow("CHIP8", SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, window_flags);

    // If failed to open
    if (!current_screen->window)
    {
        printf("Failed to open window: %s\n", SDL_GetError());
        exit(1);
    }

    // Set renderer
    current_screen->renderer = SDL_CreateRenderer(current_screen->window, -1, renderer_flags);

    // If failed to set renderer
    if (!current_screen->renderer)
    {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        exit(1);
    }
}


void prepare_scene(screen *current_screen)
{
    // Set black background
    SDL_SetRenderDrawColor(current_screen->renderer, 0, 0, 0, 0);
    SDL_RenderClear(current_screen->renderer);
    
    // Update screen
    SDL_RenderPresent(current_screen->renderer);

    // Set pixel grid to 0
    for (size_t i = 0; i < SCREEN_WIDTH; i++)
    {
        for (size_t j = 0; j < SCREEN_HEIGHT; j++)
        {
            current_screen->PIXEL_GRID[i][j] = 0;
        }
    }
}


void clear_scene(screen *current_screen)
{
    // Set black background
    SDL_SetRenderDrawColor(current_screen->renderer, 0, 0, 0, 0);
    SDL_RenderClear(current_screen->renderer);

    // Set pixel grid to 0
    for (size_t i = 0; i < SCREEN_WIDTH; i++)
    {
        for (size_t j = 0; j < SCREEN_HEIGHT; j++)
        {
            current_screen->PIXEL_GRID[i][j] = 0;
        }
    }
}


void draw_white_pixel(screen *current_screen, unsigned x, unsigned y)
{
    // Set white drawing color
    SDL_SetRenderDrawColor(current_screen->renderer, 255 ,255, 255, 255);
    
    // Draw point and update screen
    SDL_RenderDrawPoint(current_screen->renderer, x, y);
    SDL_RenderPresent(current_screen->renderer);

    // Update pixel grid
    current_screen->PIXEL_GRID[x][y] = 1;
}


void draw_black_pixel(screen *current_screen, unsigned x, unsigned y)
{
    // Set black drawing color
    SDL_SetRenderDrawColor(current_screen->renderer, 0, 0, 0, 0);
    
    // Draw point and update screen
    SDL_RenderDrawPoint(current_screen->renderer, x, y);
    SDL_RenderPresent(current_screen->renderer);

    // Update pixel grid
    current_screen->PIXEL_GRID[x][y] = 0;
}


void display_pixel_grid(screen *current_screen)
{
    for (size_t y = 0; y < SCREEN_HEIGHT; y++)
    {
        for (size_t x = 0; x < SCREEN_WIDTH; x++)
        {
            printf("%2d", current_screen->PIXEL_GRID[x][y]);
        }
        printf("\n");
    }
}