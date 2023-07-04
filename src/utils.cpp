#include "utils.h"
#include <stdlib.h>

int PixeltoFloat(int pixel, int snake_size)
{
    return pixel * snake_size;
}

int GenerateRandomNumber(int min, int max)
{
    return rand() % (max - min + 1) + min;
}
