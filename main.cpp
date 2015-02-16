#include "sdlgameeng.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_scancode.h>

#define SCREEN_WIDTH_PIXELS (2160)
#define SCREEN_HEIGHT_PIXELS (1440)
#define SCREEN_WIDTH_METERS (40)

#define FULLSCREEN

#ifdef FULLSCREEN
#define SDL_WINDOW_FLAGS    (SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN)
#else
#define SDL_WINDOW_FLAGS    (SDL_WINDOW_SHOWN)
#endif

#define PHYS_TICKS_PER_GRAPHICS_TICK (2)
#define PHYS_TIMESCALE (2)
#define DEFAULT_PHYS_DT 0.05

using namespace std;

int main( int argc, char* args[])
{

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *win = SDL_CreateWindow("PhysicsSimulator", 100, 100, SCREEN_WIDTH_PIXELS, SCREEN_HEIGHT_PIXELS, SDL_WINDOW_FLAGS);

    SDLGameEngine gameEngine(win, ((float)SCREEN_WIDTH_PIXELS)/SCREEN_WIDTH_METERS, 10);

    SDL_Surface * ballSurf = IMG_Load("ball.png");


    SDL_FreeSurface(ballSurf);

    SDL_Event gameEvent;
    bool keyStates[323] = {false};

    double phys_dt = DEFAULT_PHYS_DT;


    while(1)
    {

        phys_dt = DEFAULT_PHYS_DT; // ~60 FPS

        gameEngine.clearScreen();
        gameEngine.step(phys_dt/PHYS_TICKS_PER_GRAPHICS_TICK * PHYS_TIMESCALE);
        gameEngine.render();


        gameEngine.present();


        SDL_PollEvent(&gameEvent);

        if(gameEvent.type == SDL_QUIT)
        {
            break;
        }
        else if(gameEvent.key.type == SDL_KEYDOWN)
        {
            keyStates[gameEvent.key.keysym.scancode] = true;
        }
        else if(gameEvent.key.type == SDL_KEYUP)
        {
            keyStates[gameEvent.key.keysym.scancode] = false;
        }


        if(keyStates[SDL_Scancode::SDL_SCANCODE_W])
        {

        }
        if(keyStates[SDL_Scancode::SDL_SCANCODE_D])
        {

        }
        if(keyStates[SDL_Scancode::SDL_SCANCODE_S])
        {

        }
        if(keyStates[SDL_Scancode::SDL_SCANCODE_A])
        {

        }

        if(keyStates[SDL_Scancode::SDL_SCANCODE_UP])
        {

        }
        if(keyStates[SDL_Scancode::SDL_SCANCODE_RIGHT])
        {

        }
        if(keyStates[SDL_Scancode::SDL_SCANCODE_DOWN])
        {

        }
        if(keyStates[SDL_Scancode::SDL_SCANCODE_LEFT])
        {

        }
        if(keyStates[SDL_Scancode::SDL_SCANCODE_Q])
        {
            break;
        }
    }

    SDL_Quit();

    return 0;
}

