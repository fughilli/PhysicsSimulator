#ifndef SDLGAMEENG_H
#define SDLGAMEENG_H

#include "gameeng.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <cstdint>//#define USE_OPENGL

#ifdef USE_OPENGL
#include <gl/glew.h>
#include <gl/wglew.h>
#include <cmath>
#include <iostream>
#include <string>
#include <fstream>
#endif

class SDLGameEngine;
class SDLGameObject;

class SDLGameEngine : public GameEngine
{
public:
    SDLGameEngine(SDL_Window* window, double renderScale, int physTicksPerGameTick);
    ~SDLGameEngine();
    void step(double dt);
    void debugDrawVec(const Vector2d& origin, const Vector2d& vec, uint8_t r = 255, uint8_t g = 255, uint8_t b = 255) const;
    #ifndef USE_OPENGL
    void draw(SDL_Texture* tex, int x, int y, double angle, double scale);    #else    void drawToQuad(GLuint* tex, Vector2d* uv_rect_corners, Vector2d* quad_corners);
    void draw(GLuint* tex, int x, int y, double angle, double scale);    #endif
    void physVecToDrawCoords(const Vector2d& vec, int& x, int& y) const;
    Vector2d viewPos;
private:
    int m_w, m_h;
    double m_renderScale;
    SDL_Renderer* m_renderer;

    #ifdef USE_OPENGL

    SDL_GLContext m_glcontext;

    #endif // USE_OPENGL

    friend SDLGameObject;
};

class SDLGameObject : public GameObject
{
public:
    SDLGameObject(SDLGameEngine& gameEngine, SDL_Surface* surface, bool dynamic = true);
    ~SDLGameObject();
private:
    SDLGameEngine* m_sdlGameEngine;
    SDL_Texture* m_texture;
    virtual void draw();
    friend SDLGameEngine;
};

#endif // SDLGAMEENG_H
