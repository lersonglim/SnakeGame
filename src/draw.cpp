#include "draw.h"

// Draw green square based on position
void DrawSnake(SDL_Renderer *renderer, Snake &snake)
{
    // Draw the snake
    Node *ptr = snake.head;
    while (ptr != nullptr)
    {
        DrawPixel(renderer, ptr->x, ptr->y, snake.m_snakeSize, Color::GREEN);
        ptr = ptr->next;
    }
}

// Draw red food based on position
void DrawFood(SDL_Renderer *renderer, Food &food)
{
    // Draw the snake
    DrawPixel(renderer, food.x, food.y, food.m_snake_size, Color::RED);
}

void DrawPixel(SDL_Renderer *renderer, int x, int y, int pixel_size, Color color)
{
    switch (color)
    {
    case RED:
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        break;
    case GREEN:
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        break;
    }
    SDL_Rect rect = {x * pixel_size, y * pixel_size, pixel_size, pixel_size};
    // SDL_Rect rect = {x, y, pixel_size, pixel_size};
    SDL_RenderFillRect(renderer, &rect);
}