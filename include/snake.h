#pragma once
#include <SDL2/SDL.h>
#include "food.h"
enum Direction
{
    Left = 0,
    Right = 1,
    Up = 2,
    Down = 3,
};

class Node
{
public:
    int x;
    int y;
    Node *next;

    Node(int newX, int newY);
};

// Snake class
class Snake
{
public:
    Node *head;
    Direction direction = Right;
    int m_snakeSize;

    Snake(int initial_x, int initial_y, int snake_size);
    ~Snake();

    void eat(Food &food);

    void debug();

    Direction getDirection() const;
    void setDirection(Direction newDirection);

    void setDirection(SDL_Keycode keySymbol);

    

    Node getNextPosition();

    void move();
};
