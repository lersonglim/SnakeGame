#include <SDL2/SDL.h>
#include <iostream>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include <stdlib.h>
#include "utils.h"
#include "sdl_wrapper.h"
#include "food.h"
#include "snake.h"
#include "draw.h"
#include "collision.h"

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
const int INIT_X = WINDOW_WIDTH / 2;
const int INIT_Y = WINDOW_HEIGHT / 2;

enum GameState
{
    GameOver = -1,
    MainMenu = 0,
    Play = 1,
};

int main(int argc, char *argv[])
{
    // Initialize SDL
    InitialiseSDL();

    // Initialize TTF
    InitialiseTTF();

    // Create a window
    SDL_Window *window = CreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT);

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

    // Create a press y to restart text
    SDL_Surface *restartTS = CreateTextSurface(font, "Press Y to restart");
    // Create a texture from the surface
    SDL_Texture *restartTT = CreateTextTexture(renderer, restartTS);

    // Create a surface from the font
    SDL_Surface *replayTS = CreateTextSurface(font, "PRESS ENTER TO RESTART");
    // Create a texture from the surface
    SDL_Texture *replayTT = CreateTextTexture(renderer, gameOverTS);

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

    // Set up the destination rectangle for rendering the text
    SDL_Rect replayRect;
    replayRect.x = (WINDOW_WIDTH - textWidth) / 2;                    // Center the text horizontally
    replayRect.y = (WINDOW_HEIGHT - textHeight) / 2 - 3 * SNAKE_SIZE; // Center the text vertically
    replayRect.w = textWidth;
    replayRect.h = textHeight;

    // Snake and Food
    Snake snake(INIT_X, INIT_Y, SNAKE_SIZE);
    Food food(MIN_X, MIN_Y, WINDOW_WIDTH / SNAKE_SIZE - 1, WINDOW_HEIGHT / SNAKE_SIZE - 1, SNAKE_SIZE);

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
            bool breakEventLoop = false;

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
                    breakEventLoop = true;
                    break;
                default:
                    break;
                }
                if (breakEventLoop)
                {
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
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);
            // Render the text texture onto the screen
            SDL_RenderCopy(renderer, textTexture, nullptr, &dstRect);
            SDL_RenderPresent(renderer);
            break;
        case Play:
            // Clear the renderer
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);

            if (DetectWallCollision(snake, MIN_X, MAX_X, MIN_Y, MAX_Y) || DetectSnakeCollision(snake))
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
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);
            DrawSnake(renderer, snake);
            DrawFood(renderer, food);
            SDL_RenderCopy(renderer, restartTT, nullptr, &dstRect);
            SDL_RenderCopy(renderer, gameOverTT, nullptr, &replayRect);
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
