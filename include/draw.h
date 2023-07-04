#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "snake.h"
#include "food.h"

void DrawSnake(SDL_Renderer *renderer, Snake &snake);
void DrawFood(SDL_Renderer *renderer, Food &food);