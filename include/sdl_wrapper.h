#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

SDL_Window *CreateWindow(int window_width, int window_height);
SDL_Renderer *CreateRenderer(SDL_Window *window);
SDL_Surface *CreateTextSurface(TTF_Font *font, const char *text);
SDL_Texture *CreateTextTexture(SDL_Renderer *renderer, SDL_Surface *textSurface);
void InitialiseSDL();
void InitialiseTTF();