#include "utils.h"
#include <iostream>
#include "food.h"
// Food class

void Food::regenerate()
{
    x = GenerateRandomNumber(m_minX, m_maxX);
    y = GenerateRandomNumber(m_minY, m_maxY);
}

Food::Food(int minX, int minY, int maxX, int maxY, int snake_size)
{
    m_minX = minX;
    m_minY = minY;
    m_maxX = maxX;
    m_maxY = maxY;
    m_snake_size = snake_size;

    regenerate();
}

std::tuple<int, int> Food::get_food_position()
{
    return std::make_tuple(x, y);
}
