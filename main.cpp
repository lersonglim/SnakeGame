#include <SDL2/SDL.h>
#include <iostream>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include <stdlib.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int SNAKE_SIZE = 20;
const float SNAKE_SPEED = 15;
const int MIN_X = 0;
const int MIN_Y = 0;
const int MAX_X = WINDOW_WIDTH;
const int MAX_Y = WINDOW_HEIGHT;
const float DELAY_TIME = 1000 / SNAKE_SPEED;
// Font file path
const std::string FONT_PATH = "./debrosee.ttf";
const int INIT_X = WINDOW_WIDTH / 2;
const int INIT_Y = WINDOW_HEIGHT / 2;

int GenerateRandomNumber(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

int PixeltoFloat(int pixel)
{
    return pixel * SNAKE_SIZE;
}

enum Direction
{
    Left = 0,
    Right = 1,
    Up = 2,
    Down = 3,
};

enum GameState
{
    GameOver = -1,
    MainMenu = 0,
    Play = 1,
};

class Node
{
public:
    int x;
    int y;
    Node *next;

    Node(int newX, int newY)
    {
        x = newX;
        y = newY;
        next = nullptr;
    }
};

// Food class
class Food
{
public:
    int x;
    int y;
    Food()
    {
        x = PixeltoFloat(GenerateRandomNumber(MIN_X, WINDOW_WIDTH / SNAKE_SIZE));
        y = PixeltoFloat(GenerateRandomNumber(MIN_Y, WINDOW_HEIGHT / SNAKE_SIZE));
    }

    void regenerate()
    {
        x = PixeltoFloat(GenerateRandomNumber(MIN_X, WINDOW_WIDTH / SNAKE_SIZE - 1));
        y = PixeltoFloat(GenerateRandomNumber(MIN_Y, WINDOW_HEIGHT / SNAKE_SIZE - 1));
        std::cout << "Food position: x " << x << " y " << y << std::endl;
    }
};

// Snake class
class Snake
{
public:
    Node *head;
    Direction direction = Right;
    Snake()
    {
        head = new Node(INIT_X, INIT_Y);
        Node *node = new Node(INIT_X - SNAKE_SIZE, INIT_Y);
        head->next = node;
    }

    ~Snake()
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

    void eat(Food &food)
    {
        Node *node = new Node(food.x, food.y);
        node->next = head;
        head = node;
        food.regenerate();
    }

    void debug()
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

    Direction getDirection() const
    {
        return direction;
    }

    void setDirection(Direction newDirection)
    {
        direction = newDirection;
    }

    void setDirection(SDL_Keycode keySymbol)
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

    Node getNextPosition()
    {
        int next_x = head->x;
        int next_y = head->y;

        switch (direction)
        {
        case Up:
            // Handle up arrow key press
            next_y -= SNAKE_SIZE;
            break;
        case Down:
            // Handle down arrow key press
            next_y += SNAKE_SIZE;
            break;
        case Left:
            // Handle left arrow key press
            next_x -= SNAKE_SIZE;
            break;
        case Right:
            // Handle right arrow key press
            next_x += SNAKE_SIZE;
            break;
        default:
            break;
        }

        return Node(next_x, next_y);
    }

    void move()
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
};

// Draw green square based on position
void DrawSnake(SDL_Renderer *renderer, Snake &snake)
{
    // Draw the snake
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    Node *ptr = snake.head;
    int count = 0;
    while (ptr != nullptr)
    {
        // std::cout << "Node " << count << ": x is " << ptr->x << ", y is: " << ptr->y << std::endl;
        count += 1;
        SDL_Rect snakeRect = {ptr->x, ptr->y, SNAKE_SIZE, SNAKE_SIZE};
        SDL_RenderFillRect(renderer, &snakeRect);
        ptr = ptr->next;
    }
}

// Draw red food based on position
void DrawFood(SDL_Renderer *renderer, Food &food)
{
    // Draw the snake
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect foodRect = {food.x, food.y, SNAKE_SIZE, SNAKE_SIZE};
    SDL_RenderFillRect(renderer, &foodRect);
}

void InitialiseSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL could not initialize! SDL_Error: %s", SDL_GetError());
        exit(0);
    }
}

void InitialiseTTF()
{
    if (TTF_Init() == -1)
    {
        SDL_Log("Failed to initialize SDL_ttf: %s", TTF_GetError());
        exit(0);
    }
}

SDL_Window *CreateWindow()
{

    SDL_Window *window = SDL_CreateWindow("SDL Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window could not be created! SDL_Error: %s", SDL_GetError());
        exit(0);
    }

    return window;
}

SDL_Renderer *CreateRenderer(SDL_Window *window)
{
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Renderer could not be created! SDL_Error: %s", SDL_GetError());
        exit(0);
    }
    return renderer;
}

TTF_Font *LoadFont()
{
    TTF_Font *font = TTF_OpenFont(FONT_PATH.c_str(), 36);
    if (!font)
    {
        SDL_Log("Failed to load font: %s", TTF_GetError());
        exit(0);
    }
    return font;
}

SDL_Surface *CreateTextSurface(TTF_Font *font, const char *text)
{
    SDL_Color textColor = {255, 255, 255}; // White color
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);
    if (!textSurface)
    {
        SDL_Log("Failed to create text surface: %s", TTF_GetError());
        exit(0);
    }

    return textSurface;
}

SDL_Texture *CreateTextTexture(SDL_Renderer *renderer, SDL_Surface *textSurface)
{
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture)
    {
        SDL_Log("Failed to create text texture: %s", SDL_GetError());
        exit(0);
    }
    return textTexture;
}

bool DetectWallCollision(Snake &snake)
{
    Node posNode = snake.getNextPosition();

    if (posNode.x < MIN_X || posNode.x >= MAX_X || posNode.y < MIN_Y || posNode.y >= MAX_Y)
    {
        return true;
    }
    else
    {
        return false;
    }
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

int main(int argc, char *argv[])
{
    // Initialize SDL
    InitialiseSDL();

    // Initialize TTF
    InitialiseTTF();

    // Create a window
    SDL_Window *window = CreateWindow();

    // Create a renderer
    SDL_Renderer *renderer = CreateRenderer(window);

    // Main Menu
    // Load the font
    TTF_Font *font = LoadFont();
    // Create a surface from the font
    SDL_Surface *textSurface = CreateTextSurface(font, "PRESS ENTER TO PLAY THE GAME");
    // Create a texture from the surface
    SDL_Texture *textTexture = CreateTextTexture(renderer, textSurface);

    // Create a surface from the font
    SDL_Surface *gameOverTS = CreateTextSurface(font, "GAME OVER");
    // Create a texture from the surface
    SDL_Texture *gameOverTT = CreateTextTexture(renderer, gameOverTS);

    // Get the dimensions of the text surface
    int textWidth = textSurface->w;
    int textHeight = textSurface->h;

    // Clean up the surface as it is no longer needed
    SDL_FreeSurface(textSurface);

    // Set up the destination rectangle for rendering the text
    SDL_Rect dstRect;
    dstRect.x = (WINDOW_WIDTH - textWidth) / 2;   // Center the text horizontally
    dstRect.y = (WINDOW_HEIGHT - textHeight) / 2; // Center the text vertically
    dstRect.w = textWidth;
    dstRect.h = textHeight;

    // Snake and Food
    Snake snake;
    Food food;

    // Main loop
    bool quit = false;
    GameState gamestate = MainMenu;
    SDL_Event event;
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    while (!quit)
    {
        // Handle events

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_KEYDOWN)
            {
                switch (gamestate)
                {
                case MainMenu:
                    if (event.key.keysym.sym == SDLK_RETURN)
                    {
                        gamestate = Play;
                        SDL_DestroyTexture(textTexture);
                    }
                    break;
                case Play:
                    snake.setDirection(event.key.keysym.sym);
                    break;
                default:
                    break;
                }
            }
            else if (event.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        switch (gamestate)
        {
        case MainMenu:
            // Clear the renderer
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            // Render the text texture onto the screen
            SDL_RenderCopy(renderer, textTexture, nullptr, &dstRect);
            SDL_RenderPresent(renderer);
            break;
        case Play:
            // Clear the renderer
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            if (DetectWallCollision(snake))
            {
                gamestate = GameState::GameOver;
            }
            else if (DetectFoodCollision(snake, food))
            {
                snake.eat(food);
            }
            else
            {
                // Update snake position
                snake.move();
            }
            // Draw
            DrawSnake(renderer, snake);
            DrawFood(renderer, food);

            // Delay for a short time
            SDL_RenderPresent(renderer);
            SDL_Delay(DELAY_TIME);
            break;
        case GameOver:
            // Clear the renderer
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            DrawSnake(renderer, snake);
            DrawFood(renderer, food);
            SDL_RenderCopy(renderer, gameOverTT, nullptr, &dstRect);
            SDL_RenderPresent(renderer);
            break;
        default:
            break;
        }
    }

    // Cleanup and quit SDL
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
