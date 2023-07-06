#include "utils.h"
#include "sdl_wrapper.h"
#include "food.h"
#include "snake.h"
#include "draw.h"
#include "collision.h"
#include "text.h"

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
    Text game_over_text("GAME OVER", renderer, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 3 * SNAKE_SIZE);
    Text restart_text("Press Y to restart", renderer, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

    std::unique_ptr<Text> main_menu_text;
    std::unique_ptr<Snake> snake;
    std::unique_ptr<Food> food;

    // Main loop
    bool quit = false;
    bool initialise = true;
    GameState gamestate = MainMenu;
    SDL_Event event;
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    while (!quit)
    {
        // Initialise and Reinitialise
        if (initialise)
        {

            snake.reset(new Snake(INIT_X, INIT_Y, SNAKE_SIZE));
            food.reset(new Food(MIN_X, MIN_Y, WINDOW_WIDTH / SNAKE_SIZE - 1, WINDOW_HEIGHT / SNAKE_SIZE - 1, SNAKE_SIZE));
            main_menu_text.reset(new Text("PRESS ENTER TO PLAY THE GAME", renderer, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));
            initialise = false;
        }

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

                        main_menu_text->destroy();
                    }
                    break;
                case Play:
                    snake->setDirection(event.key.keysym.sym);
                    breakEventLoop = true;
                    break;
                case GameOver:
                    if (event.key.keysym.sym == SDLK_y)
                    {
                        gamestate = MainMenu;
                        initialise = true;
                    }
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
            main_menu_text->render();
            SDL_RenderPresent(renderer);
            break;
        case Play:
            // Clear the renderer
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);

            if (DetectWallCollision(*snake, MIN_X, MAX_X, MIN_Y, MAX_Y) || DetectSnakeCollision(*snake))
            {
                gamestate = GameState::GameOver;
            }
            else if (DetectFoodCollision(*snake, *food))
            {
                snake->eat(*food);
            }
            else
            {
                // Update snake position
                snake->move();
            }
            // Draw
            DrawSnake(renderer, *snake);
            DrawFood(renderer, *food);

            // Delay for a short time
            SDL_RenderPresent(renderer);
            SDL_Delay(DELAY_TIME);
            break;
        case GameOver:
            // Clear the renderer
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);
            DrawSnake(renderer, *snake);
            DrawFood(renderer, *food);
            restart_text.render();
            game_over_text.render();
            SDL_RenderPresent(renderer);
            break;
        default:
            break;
        }
    }
    // Cleanup and quit SDL
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
