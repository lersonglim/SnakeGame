#pragma once
#include "utils.h"
#include <iostream>
// Food class
class Food
{
public:
    int x;
    int y;
    int m_minX;
    int m_minY;
    int m_maxX; // equals to WINDOW_WIDTH / SNAKE_SIZE - 1
    int m_maxY; // equals to WINDOW_HEIGHT / SNAKE_SIZE - 1
    int m_snake_size;

    Food(int minX, int minY, int maxX, int maxY, int snake_size);

    void regenerate();

    std::tuple<int, int> get_food_position();
};