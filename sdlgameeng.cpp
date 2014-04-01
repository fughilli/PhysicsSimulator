#include "sdlgameeng.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "Vector/Vector.h"

//SDLGameEngine::SDLGameEngine(SDL_Renderer* renderer, int w, int h, double renderScale, int physTicksPerGameTick) :
//    GameEngine(physTicksPerGameTick), m_renderScale(renderScale), m_renderer(renderer), m_w(w), m_h(h)
//{
//
//}

SDLGameEngine::SDLGameEngine(SDL_Window* window, double renderScale, int physTicksPerGameTick) :
    GameEngine(physTicksPerGameTick), m_renderScale(renderScale)
{
    m_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_GetWindowSize(window, &m_w, &m_h);
    viewPos = Vector2d::zero;
}

void SDLGameEngine::step(double dt)
{
    SDL_RenderClear(m_renderer);

    GameEngine::step(dt);

    SDL_RenderPresent(m_renderer);
}

//SDLGameObject::SDLGameObject(SDLGameEngine& gameEngine, SDL_Texture* texture) :
//    GameObject(gameEngine), m_texture(texture), m_sdlGameEngine(&gameEngine)
//{
//
//}

SDLGameObject::SDLGameObject(SDLGameEngine& gameEngine, SDL_Surface* surface, bool dynamic) :
    GameObject(gameEngine, dynamic), m_sdlGameEngine(&gameEngine)
{
    m_texture = SDL_CreateTextureFromSurface(gameEngine.m_renderer, surface);
}

void SDLGameObject::draw()
{
    int posx = ((pos.x - m_sdlGameEngine->viewPos.x) * m_sdlGameEngine->m_renderScale) + m_sdlGameEngine->m_w/2;
    int posy = m_sdlGameEngine->m_h/2 - ((pos.y - m_sdlGameEngine->viewPos.y) * m_sdlGameEngine->m_renderScale);
    m_sdlGameEngine->draw(m_texture, posx, posy, angle, 2.8*radius);
}

void SDLGameEngine::debugDrawVec(const Vector2d& origin, const Vector2d& vec, uint8_t r, uint8_t g, uint8_t b) const
{
    Uint8 temp_r, temp_g, temp_b, temp_a;
    SDL_GetRenderDrawColor(m_renderer, &temp_r, &temp_g, &temp_b, &temp_a);
    SDL_SetRenderDrawColor(m_renderer, r, g, b, 0xFF);
    int x1, x2, y1, y2;
    physVecToDrawCoords(origin, x1, y1);
    physVecToDrawCoords(origin + vec, x2, y2);
    SDL_RenderDrawLine(m_renderer, x1, y1, x2, y2);
    SDL_Rect endptBox;
    endptBox.x = x2 - 2;
    endptBox.y = y2 - 2;
    endptBox.w = 4;
    endptBox.h = 4;
    SDL_RenderDrawRect(m_renderer, &endptBox);

    SDL_SetRenderDrawColor(m_renderer, temp_r, temp_g, temp_b, temp_a);
}

void SDLGameEngine::physVecToDrawCoords(const Vector2d& vec, int& x, int& y) const
{
    x = ((vec.x - viewPos.x) * m_renderScale) + m_w/2;
    y = m_h/2 - ((vec.y - viewPos.y) * m_renderScale);
}


void SDLGameEngine::draw(SDL_Texture* tex, int x, int y, double angle, double scale)
{
    SDL_Rect destRect;

    SDL_QueryTexture(tex, NULL, NULL, &destRect.w, &destRect.h);
    destRect.w*=scale;
    destRect.h*=scale;
    destRect.x = x - destRect.w/2;
    destRect.y = y - destRect.h/2;

    SDL_RenderCopyEx(m_renderer, tex, NULL, &destRect, angle/180*PI, NULL, SDL_FLIP_NONE);
}

SDLGameEngine::~SDLGameEngine()
{
    SDL_DestroyRenderer(m_renderer);
}

SDLGameObject::~SDLGameObject()
{
    SDL_DestroyTexture(m_texture);
}


