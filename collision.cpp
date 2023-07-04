#include "snake.h"

bool DetectWallCollision(Snake &snake, int min_x, int max_x, int min_y, int max_y)
{
    Node posNode = snake.getNextPosition();

    if (posNode.x < min_x || posNode.x >= max_x || posNode.y < min_y || posNode.y >= max_y)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool DetectSnakeCollision(Snake &snake)
{
    Node posNode = snake.getNextPosition();

    Node *ptr = snake.head;
    ptr = ptr->next;

    while (ptr != nullptr)
    {
        if (posNode.x == ptr->x && posNode.y == ptr->y)
        {
            return true;
        }
        ptr = ptr->next;
    }
    return false;
}

bool DetectFoodCollision(Snake &snake, Food &food)
{
    Node posNode = snake.getNextPosition();
    if (posNode.x == food.x && posNode.y == food.y)
    {
        return true;
    }
    else
    {
        return false;
    }
}