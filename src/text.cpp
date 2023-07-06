#include "text.h"

const int FONT_SIZE = 36;

TTF_Font *LoadFont()
{
    TTF_Font *font = TTF_OpenFont(FONT_PATH.c_str(), FONT_SIZE);
    if (!font)
    {
        SDL_Log("Failed to load font: %s", TTF_GetError());
        exit(0);
    }
    return font;
}

Text::Text(const char *text, SDL_Renderer *renderer, int position_x, int position_y)
{
    m_font = LoadFont();
    m_textSurface = CreateTextSurface(m_font, text);
    m_textTexture = CreateTextTexture(renderer, m_textSurface);
    m_renderer = renderer;

    // Set up the destination rectangle for rendering the text
    m_dstRect.x = position_x - m_textSurface->w / 2; // To adjust based on text length
    m_dstRect.y = position_y - m_textSurface->h / 2; // To adjust based on text length
    m_dstRect.w = m_textSurface->w;
    m_dstRect.h = m_textSurface->h;
}

Text::~Text()
{
    SDL_FreeSurface(m_textSurface);
    // TTF_CloseFont(m_font); // This causes segmentation fault upon exiting the SDL window
}

void Text::render()
{
    SDL_RenderCopy(m_renderer, m_textTexture, nullptr, &m_dstRect);
}

void Text::destroy()
{
    SDL_DestroyTexture(m_textTexture);
}
