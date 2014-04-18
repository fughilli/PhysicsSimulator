#include "sdlgameeng.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "Vector/Vector.h"

//#define USE_OPENGL

SDLGameEngine::SDLGameEngine(SDL_Window* window, double renderScale, int physTicksPerGameTick) :
    GameEngine(physTicksPerGameTick), m_renderScale(renderScale)
{
#ifndef USE_OPENGL
    m_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
#endif
    SDL_GetWindowSize(window, &m_w, &m_h);

#ifdef USE_OPENGL

    m_glcontext = SDL_GL_CreateContext(window);

    glewInit();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0.0f, m_w, m_h, 0.0f, -100.0f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glViewport(0, 0, m_w, m_h);

#endif // USE_OPENGL
    viewPos = Vector2d::zero;
}

void SDLGameEngine::step(double dt)
{
#ifdef USE_OPENGL
    glUseProgram(m_shaderProgram);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,1);

    glPushMatrix();

    glTranslatef(m_w/2, m_h/2, 0.0f);

    GameEngine::step();
    //glDrawElements(GL_TRIANGLES, numindices, GL_UNSIGNED_INT, vertexindices);

    glUseProgram(0);

    glPopMatrix();

    SDL_GL_SwapWindow(window);
#else
    SDL_RenderClear(m_renderer);

    GameEngine::step(dt);

    SDL_RenderPresent(m_renderer);
#endif
}

SDLGameObject::SDLGameObject(SDLGameEngine& gameEngine, SDL_Surface* surface, bool dynamic) :
    GameObject(gameEngine, dynamic), m_sdlGameEngine(&gameEngine)
{
    m_texture = SDL_CreateTextureFromSurface(gameEngine.m_renderer, surface);

#ifdef USE_OPENGL

    glGenTextures(1, &m_glTexture);
    glBindTexture(GL_TEXTURE_2D, m_glTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D, 0, m_texture->format->BytesPerPixel,
        m_texture->w, m_texture->h,
        0, (m_texture->format->BytesPerPixel == 4)?(GL_RGBA):(GL_RGB), GL_UNSIGNED_BYTE,
        m_texture->pixels);

#endif // USE_OPENGL
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


