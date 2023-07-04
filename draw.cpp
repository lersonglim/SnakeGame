#include "draw.h"

// Draw green square based on position
void DrawSnake(SDL_Renderer *renderer, Snake &snake)
{
    // Draw the snake
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    Node *ptr = snake.head;
    int count = 0;
    while (ptr != nullptr)
    {
        // std::cout << "Node " << count << ": x is " << ptr->x << ", y is: " << ptr->y << std::endl;
        count += 1;
        SDL_Rect snakeRect = {ptr->x, ptr->y, snake.m_snakeSize, snake.m_snakeSize};
        SDL_RenderFillRect(renderer, &snakeRect);
        ptr = ptr->next;
    }
}

// Draw red food based on position
void DrawFood(SDL_Renderer *renderer, Food &food)
{
    // Draw the snake
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect foodRect = {food.x, food.y, food.m_snake_size, food.m_snake_size};
    SDL_RenderFillRect(renderer, &foodRect);
}