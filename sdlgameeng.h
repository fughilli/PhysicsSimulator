#ifndef SDLGAMEENG_H
#define SDLGAMEENG_H

#include "gameeng.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <cstdint>

class SDLGameEngine;
class SDLGameObject;

class SDLGameEngine : public GameEngine
{
public:
    //SDLGameEngine(SDL_Renderer* renderer, double renderScale, int physTicksPerGameTick);
    SDLGameEngine(SDL_Window* window, double renderScale, int physTicksPerGameTick);
    ~SDLGameEngine();
    void step(double dt);
    void debugDrawVec(const Vector2d& origin, const Vector2d& vec, uint8_t r = 255, uint8_t g = 255, uint8_t b = 255) const;
    void draw(SDL_Texture* tex, int x, int y, double angle, double scale);
    void physVecToDrawCoords(const Vector2d& vec, int& x, int& y) const;
    Vector2d viewPos;
private:
    int m_w, m_h;
    double m_renderScale;
    SDL_Renderer* m_renderer;
    friend SDLGameObject;
};

class SDLGameObject : public GameObject
{
public:
    //SDLGameObject(SDLGameEngine& gameEngine, SDL_Texture* texture);
    SDLGameObject(SDLGameEngine& gameEngine, SDL_Surface* surface, bool dynamic = true);
    ~SDLGameObject();
private:
    SDLGameEngine* m_sdlGameEngine;
    SDL_Texture* m_texture;
    virtual void draw();
    friend SDLGameEngine;
};

#endif // SDLGAMEENG_H
