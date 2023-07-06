#pragma once
#include <iostream>
#include <SDL2/SDL_ttf.h>
#include "sdl_wrapper.h"
const std::string FONT_PATH = "assets/debrosee.ttf";
TTF_Font *LoadFont();

class Text
{
public:
    SDL_Surface *m_textSurface;
    SDL_Texture *m_textTexture;
    std::string m_text;
    SDL_Renderer *m_renderer;
    SDL_Rect m_dstRect;
    TTF_Font *m_font;

    Text(const char *text, SDL_Renderer *renderer, int position_x, int position_y);
    ~Text();

    void render();

    void destroy();
};
