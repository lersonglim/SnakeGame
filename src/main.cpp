#include "utils.h"
#include "sdl_wrapper.h"
#include "food.h"
#include "snake.h"
#include "draw.h"
#include "collision.h"
#include "text.h"
#include <nlohmann/json.hpp>

// Boost Libraries
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = net::ip::tcp;

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 500;
const int PIXEL_SIZE = 20;
const float SNAKE_SPEED = 15;
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

class State
{
public:
    std::vector<std::tuple<int, int>> snake_position;
    std::tuple<int, int> food_position;
    Direction snake_direction;
    GameState game_state;
    int score;

    // Constructor
    State(std::vector<std::tuple<int, int>> s_pos, std::tuple<int, int> f_pos, Direction s_dir, GameState g_state, int s) : snake_position(s_pos), food_position(f_pos), snake_direction(s_dir), game_state(g_state), score(s) {}

    nlohmann::json json()
    {
        nlohmann::json jsonState;
        jsonState["snake_position"] = snake_position;
        jsonState["food_position"] = food_position;
        jsonState["snake_direction"] = static_cast<int>(snake_direction);
        jsonState["game_state"] = static_cast<int>(game_state);
        jsonState["score"] = static_cast<int>(score);

        return jsonState;
    };
};

std::shared_ptr<websocket::stream<tcp::socket>>
create_websocket_connection(std::string host, std::string port)
{
    net::io_context ioc;
    tcp::resolver resolver{ioc};
    std::shared_ptr<websocket::stream<tcp::socket>> ws = std::make_shared<websocket::stream<tcp::socket>>(ioc);

    auto const results = resolver.resolve(host, port);
    auto ep = net::connect(ws->next_layer(), results);

    host += ':' + std::to_string(ep.port());

    ws->handshake(host, "/websocket");

    return ws;
}

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
    auto ws = create_websocket_connection("localhost", "8888");

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

            // Delay for a short time
            SDL_RenderPresent(renderer);

            // update state and websocket send state
            {
                State state(snake->get_node_positions(), food->get_food_position(), snake->getDirection(), gamestate, score);
                std::string jsonString = state.json().dump(4); // Use pretty printing with an indentation of 4 spaces
                std::cout << jsonString << std::endl;
                ws->write(net::buffer(jsonString));
            }

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
    ws->close(websocket::close_code::normal);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
