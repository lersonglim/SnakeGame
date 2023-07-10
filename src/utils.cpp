#include "utils.h"
#include <cstdlib>
#include <vector>
#include <tuple>
#include <iostream>

int PixeltoFloat(int pixel, int snake_size)
{
    return pixel * snake_size;
}

int GenerateRandomNumber(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

void print(std::tuple<int, int> tuple)
{
    int x;
    int y;
    std::tie(x, y) = tuple;
    std::cout << "[(" << x << "," << y << ")]" << std::endl;
}

void print(std::vector<std::tuple<int, int>> vector_tuples)
{
    std::cout << "[";
    for (size_t i = 0; i < vector_tuples.size(); ++i)
    {
        int x;
        int y;
        std::tie(x, y) = vector_tuples[i];
        std::cout << "(" << x << ", " << y << ")";
        if (i != vector_tuples.size() - 1)
        {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
}
