#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "snake.h"
#include "food.h"

enum Color
{
    RED,
    GREEN
    // Add more colors as needed
};

void DrawSnake(SDL_Renderer *renderer, Snake &snake);
void DrawFood(SDL_Renderer *renderer, Food &food);
void DrawPixel(SDL_Renderer *renderer, int x, int y, int pixel_size, Color color);