#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>



SDL_Window *CreateWindow(int window_width, int window_height)
{

    SDL_Window *window = SDL_CreateWindow("SDL Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_SHOWN);

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

SDL_Surface *CreateTextSurface(TTF_Font *font, const char *text)
{
    SDL_Color textColor = {0, 0, 0}; // Black Color
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
