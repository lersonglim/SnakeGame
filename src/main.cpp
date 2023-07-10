#include "utils.h"
#include "sdl_wrapper.h"
#include "food.h"
#include "snake.h"
#include "draw.h"
#include "collision.h"
#include "text.h"

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 500;
const int PIXEL_SIZE = 20;
const float SNAKE_SPEED = 12;
const int MIN_X = 0;
const int MIN_Y = 0;
const int MAX_X = WINDOW_WIDTH / PIXEL_SIZE - 1; // without -1 this will be the edge of the window
const int MAX_Y = WINDOW_HEIGHT / PIXEL_SIZE - 1;
const float DELAY_TIME = 1000 / SNAKE_SPEED;
// Font file path

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
    Text game_over_text("GAME OVER", renderer, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 3 * PIXEL_SIZE);
    Text restart_text("Press Y to restart", renderer, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

    std::unique_ptr<Text> main_menu_text;
    std::unique_ptr<Text> score_text;
    std::unique_ptr<Snake> snake;
    std::unique_ptr<Food> food;

    // Main loop
    bool quit = false;
    bool initialise = true;
    int score = 0;
    GameState gamestate = MainMenu;
    SDL_Event event;
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    while (!quit)
    {
        // Initialise and Reinitialise
        if (initialise)
        {
            score = 0;
            snake.reset(new Snake(MAX_X / 2, MAX_Y / 2, PIXEL_SIZE));
            food.reset(new Food(MIN_X, MIN_Y, MAX_X, MAX_Y, PIXEL_SIZE));
            main_menu_text.reset(new Text("PRESS ENTER TO PLAY THE GAME", renderer, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));

            std::string scoreString = "Score: " + std::to_string(score);
            score_text.reset(new Text(scoreString.c_str(), renderer, WINDOW_WIDTH / 2, 20));
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
                        score_text->destroy();
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

            // MAX_X +1 and MAX_Y + 1 because that's the edge. MAX_X is the last pixel
            if (DetectWallCollision(*snake, MIN_X, MAX_X + 1, MIN_Y, MAX_Y + 1) || DetectSnakeCollision(*snake))
            {
                gamestate = GameState::GameOver;
            }
            else if (DetectFoodCollision(*snake, *food))
            {
                snake->eat(*food);
                score += 1;
                std::string scoreString = "Score: " + std::to_string(score);
                score_text.reset(new Text(scoreString.c_str(), renderer, WINDOW_WIDTH / 2, 20));
            }
            else
            {
                // Update snake position
                snake->move();
            }
            // Draw

            score_text->render();
            DrawSnake(renderer, *snake);

            DrawFood(renderer, *food);

            // print(snake->get_node_positions());
            print(food->get_food_position());

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
