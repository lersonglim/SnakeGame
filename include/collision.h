#pragma once
#include "snake.h"

bool DetectWallCollision(Snake &snake, int min_x, int max_x, int min_y, int max_y);
bool DetectSnakeCollision(Snake &snake);
bool DetectFoodCollision(Snake &snake, Food &food);
