#include <iostream>
#include <SDL2/SDL.h>
#include "food.h"
#include "snake.h"


Node::Node(int newX, int newY)
{
    x = newX;
    y = newY;
    next = nullptr;
}

// Snake class
Snake::Snake(int initial_x, int initial_y, int snake_size)
{
    m_snakeSize = snake_size;
    head = new Node(initial_x, initial_y);
    Node *node = new Node(initial_x - m_snakeSize, initial_y);
    head->next = node;
}

Snake::~Snake()
{
    // TODO: iteratively delete a linked list
    Node *ptr = head;
    while (ptr != nullptr)
    {
        Node *ptr_to_be_deleted = ptr;
        ptr = ptr->next;
        delete ptr_to_be_deleted;
    }
}

void Snake::eat(Food &food)
{
    Node *node = new Node(food.x, food.y);
    node->next = head;
    head = node;
    food.regenerate();
}

void Snake::debug()
{
    Node *ptr = head;
    int count = 0;

    while (ptr != nullptr)
    {
        std::cout << "Node " << count << ": x is " << ptr->x << ", y is: " << ptr->y << std::endl;
        count += 1;
        ptr = ptr->next;
    }
}

Direction Snake::getDirection() const
{
    return direction;
}

void Snake::setDirection(Direction newDirection)
{
    direction = newDirection;
}

void Snake::setDirection(SDL_Keycode keySymbol)
{
    switch (keySymbol)
    {
    case SDLK_UP:
        // Handle up arrow key press
        if (direction != Direction::Down)
        {
            setDirection(Direction::Up);
        }
        break;
    case SDLK_DOWN:
        // Handle down arrow key press
        if (direction != Direction::Up)
        {
            setDirection(Direction::Down);
        }
        break;
    case SDLK_LEFT:
        // Handle left arrow key press
        if (direction != Direction::Right)
        {
            setDirection(Direction::Left);
        }

        break;
    case SDLK_RIGHT:
        // Handle right arrow key press
        if (direction != Direction::Left)
        {
            setDirection(Direction::Right);
        }
        break;
    default:
        break;
    }
}

Node Snake::getNextPosition()
{
    int next_x = head->x;
    int next_y = head->y;

    switch (direction)
    {
    case Up:
        // Handle up arrow key press
        next_y -= m_snakeSize;
        break;
    case Down:
        // Handle down arrow key press
        next_y += m_snakeSize;
        break;
    case Left:
        // Handle left arrow key press
        next_x -= m_snakeSize;
        break;
    case Right:
        // Handle right arrow key press
        next_x += m_snakeSize;
        break;
    default:
        break;
    }

    return Node(next_x, next_y);
}

void Snake::move()
{

    // Before changing head position shift every past links to its next position
    Node *ptr = head;
    int newX = ptr->x;
    int newY = ptr->y;

    int oldX = 0;
    int oldY = 0;

    while (true)
    {
        // Update the next pointer value from the past values
        ptr = ptr->next;
        if (ptr == nullptr)
        {
            break;
        }
        oldX = ptr->x;
        oldY = ptr->y;
        ptr->x = newX;
        ptr->y = newY;

        newX = oldX;
        newY = oldY;
    }

    Node posNode = getNextPosition();
    head->y = posNode.y;
    head->x = posNode.x;
}
