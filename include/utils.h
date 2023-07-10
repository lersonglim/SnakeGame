#pragma once

#include <vector>
#include <tuple>

int PixeltoFloat(int pixel, int snake_size);
int GenerateRandomNumber(int min, int max);
void print(std::tuple<int, int> tuple);
void print(std::vector<std::tuple<int, int>> vector_tuples);