#include "utils.h"
#include <iostream>
#include "food.h"
// Food class

void Food::regenerate()
{
    x = PixeltoFloat(GenerateRandomNumber(m_minX, m_maxX), m_snake_size);
    y = PixeltoFloat(GenerateRandomNumber(m_minY, m_maxY), m_snake_size);
    std::cout << "Food position: x " << x << " y " << y << std::endl;
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
