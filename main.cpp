#include "sdlgameeng.h"#include <iostream>#include <SDL2/SDL.h>#include <SDL2/SDL_image.h>#include <SDL2/SDL_scancode.h>#define HD_RES#ifdef HD_RES#define SCREEN_WIDTH_PIXELS (640)#define SCREEN_HEIGHT_PIXELS (480)#define SCREEN_WIDTH_METERS (40)#else#define SCREEN_WIDTH_PIXELS (1280)#define SCREEN_HEIGHT_PIXELS (800)#define SCREEN_WIDTH_METERS (26)#endif//#define FULLSCREEN#ifdef FULLSCREEN#define SDL_WINDOW_FLAGS    (SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN)#else#define SDL_WINDOW_FLAGS    (SDL_WINDOW_SHOWN)#endif#define PHYS_TICKS_PER_GRAPHICS_TICK (2)#define PHYS_TIMESCALE (1)using namespace std;//#define CIRCULAR_GRAVITY_TEST//#define ROPE_TEST#define CLOTH_TEST#define CONTROL_BOTH_ENDS//#define REAL_TIME#define WORLD_GRAVITY//#define KEY_TO_STEPint main( int argc, char* args[]){    SDL_Init(SDL_INIT_EVERYTHING);    SDL_Window *win = SDL_CreateWindow("PhysicsSimulator", 100, 100, SCREEN_WIDTH_PIXELS, SCREEN_HEIGHT_PIXELS, SDL_WINDOW_FLAGS);    SDLGameEngine gameEngine(win, ((float)SCREEN_WIDTH_PIXELS)/SCREEN_WIDTH_METERS, 10);    SDL_Surface * ballSurf = IMG_Load("ball.png");#ifdef CIRCULAR_GRAVITY_TEST    const int numBalls = 10;    const int numWallBalls = 70;    SDLGameObject *balls[numBalls];    SDLGameObject *wall[numWallBalls];    for(int i = 0; i < numBalls; i++)    {        balls[i] = new SDLGameObject(gameEngine, ballSurf);        balls[i]->radius = 0.5;        balls[i]->mass = 1e12;        balls[i]->pos = Vector2d::i.rotate(TWO_PI*i/numBalls)*((float)i*9/numBalls);        balls[i]->vel = balls[i]->pos.rotate(PI/2)*2;        balls[i]->selfGravity = true;    }    for(int i = 0; i < numWallBalls; i++)    {        wall[i] = new SDLGameObject(gameEngine, ballSurf, false);        wall[i]->radius = 0.5;        wall[i]->mass = 1;        wall[i]->pos = Vector2d::i.rotate(TWO_PI*i/numWallBalls)*11;    }#endif#ifdef SQUARE_TEST    const int numBalls = 10;    SDLGameObject *balls[numBalls*numBalls];    for(int i = 0; i < numBalls; i++)    {        for(int j = 0; j < numBalls; j++)        {            balls[i*numBalls + j] = new SDLGameObject(gameEngine, ballSurf);            balls[i*numBalls + j]->radius = 0.5;            balls[i*numBalls + j]->mass = 5;            balls[i*numBalls + j]->pos = Vector2d(i*2 - numBalls, j*2 - numBalls);            balls[i*numBalls + j]->vel = Vector2d::zero;            balls[i*numBalls + j]->selfGravity = false;        }    }    balls[2]->dynamic = false;#endif#ifdef WORLD_GRAVITY    ForceField *myForceField = new ForceField(gameEngine);    myForceField->fieldAccel = Vector2d(0, -9.8);#endif#ifdef FOUR_ORBIT_TEST    SDLGameObject *balls[5];    balls[0] = new SDLGameObject(gameEngine, ballSurf);    balls[1] = new SDLGameObject(gameEngine, ballSurf);    balls[2] = new SDLGameObject(gameEngine, ballSurf);    balls[3] = new SDLGameObject(gameEngine, ballSurf);    balls[4] = new SDLGameObject(gameEngine, ballSurf);    balls[0]->pos = Vector2d(0, 0);    balls[0]->vel = Vector2d::zero;    balls[0]->mass = 1e13;    balls[0]->radius = 1;    balls[0]->angvel = 50;    balls[0]->selfGravity = true;    balls[1]->pos = Vector2d(-10, 0);    balls[1]->vel = (Vector2d::j+Vector2d::i)*5.5;    balls[1]->mass = 1e13;    balls[1]->radius = 0.5;    balls[2]->pos = Vector2d(10, 0);    balls[2]->vel = (Vector2d::j+Vector2d::i)*-5.5;    balls[2]->mass = 1e13;    balls[2]->radius = 0.5;    balls[3]->pos = Vector2d(0, 10);    balls[3]->vel = (Vector2d::i-Vector2d::j)*5.5;    balls[3]->mass = 1e13;    balls[3]->radius = 0.5;    balls[4]->pos = Vector2d(0, -10);    balls[4]->vel = (Vector2d::i-Vector2d::j)*-5.5;    balls[4]->mass = 1e13;    balls[4]->radius = 0.5;#endif#ifdef ROPE_TEST    const int chain_length = 30;    const double chain_link_length = 0.4;    double total_chain_length = chain_length * chain_link_length;    SDLGameObject* balls[chain_length + 1];    RopeConstraint* rpConsts[chain_length - 1];    balls[chain_length] = new SDLGameObject(gameEngine, ballSurf, false);    balls[chain_length]->mass = 0.1;    balls[chain_length]->radius = 3;    balls[chain_length]->pos = Vector2d(0, 5);    balls[0] = new SDLGameObject(gameEngine, ballSurf, false);    balls[0]->mass = 1;    balls[0]->radius = 0.1;    balls[0]->pos = Vector2d(-total_chain_length/2, 0);#ifdef CONTROL_BOTH_ENDS    balls[chain_length - 1] = new SDLGameObject(gameEngine, ballSurf, false);#else    balls[chain_length - 1] = new SDLGameObject(gameEngine, ballSurf, true);#endif    balls[chain_length - 1]->mass = 1;    balls[chain_length - 1]->radius = 0.1;    balls[chain_length - 1]->pos = Vector2d(total_chain_length/2, 0);    for(int i = 1; i < chain_length - 1; i++)    {        balls[i] = new SDLGameObject(gameEngine, ballSurf);        balls[i]->mass = 1;        balls[i]->radius = 0.1;        balls[i]->pos = balls[i-1]->pos + Vector2d(chain_link_length, 0);        rpConsts[i-1] = new RopeConstraint(gameEngine, balls[i-1], balls[i], chain_link_length);    }    rpConsts[chain_length - 2] = new RopeConstraint(gameEngine, balls[chain_length - 2], balls[chain_length - 1], chain_link_length);#endif // ROPE_TEST#ifdef CLOTH_TEST    const int clothWidth = 10, clothHeight = 10;    SDLGameObject* balls[clothWidth][clothHeight];    PhysicsConstraint* rpConsts[2][clothWidth][clothHeight];    for(int i = 0; i < clothWidth; i++)    {        for(int j = 0; j < clothHeight; j++)        {            balls[i][j] = new SDLGameObject(gameEngine, ballSurf, true);            balls[i][j]->pos = Vector2d(i, -j);            balls[i][j]->mass = 1;            balls[i][j]->radius = 0.1;            balls[i][j]->selfGravity = false;            balls[i][j]->noCollide = true;        }    }    balls[0][0]->dynamic = false;#ifdef CONTROL_BOTH_ENDS    balls[clothWidth-1][0]->dynamic = false;#endif    for(int i = 0; i < clothWidth; i++)    {        for(int j = 1; j < clothHeight; j++)        {            rpConsts[0][i][j] = new RopeConstraint(gameEngine, balls[i][j-1], balls[i][j], 1);        }    }    for(int j = 0; j < clothHeight; j++)    {        for(int i = 1; i < clothWidth; i++)        {            rpConsts[1][i][j] = new RopeConstraint(gameEngine, balls[i-1][j], balls[i][j], 1);        }    }#endif // CLOTH_TEST    SDL_FreeSurface(ballSurf);#ifdef REAL_TIME    Uint32 oldTime, curTime = 0;    curTime = SDL_GetTicks();#endif    SDL_Event gameEvent;    bool keyStates[323] = {false};    double phys_dt = 0.05;    while(1)    {#ifdef REAL_TIME        oldTime = curTime;        curTime = SDL_GetTicks();#else        phys_dt = 0.05; // ~60 FPS#endif#ifdef KEY_TO_STEP        if(keyStates[SDL_Scancode::SDL_SCANCODE_SPACE])        {#endif            gameEngine.step(phys_dt/PHYS_TICKS_PER_GRAPHICS_TICK * PHYS_TIMESCALE);            #ifdef REAL_TIME            phys_dt = (curTime - oldTime)/1000.0;            std::cerr << phys_dt << std::endl;            #endif // REAL_TIME#ifdef KEY_TO_STEP            SDL_Delay(100);        }#endif        SDL_PollEvent(&gameEvent);        if(gameEvent.type == SDL_QUIT)        {            break;        }        else if(gameEvent.key.type == SDL_KEYDOWN)        {            keyStates[gameEvent.key.keysym.scancode] = true;        }        else if(gameEvent.key.type == SDL_KEYUP)        {            keyStates[gameEvent.key.keysym.scancode] = false;        }        Vector2d dv1 = Vector2d::zero;        Vector2d dv2 = Vector2d::zero;        if(keyStates[SDL_Scancode::SDL_SCANCODE_W])        {            dv1 += Vector2d::j/phys_dt;        }        if(keyStates[SDL_Scancode::SDL_SCANCODE_D])        {            dv1 += Vector2d::i/phys_dt;        }        if(keyStates[SDL_Scancode::SDL_SCANCODE_S])        {            dv1 += Vector2d::j*-1/phys_dt;        }        if(keyStates[SDL_Scancode::SDL_SCANCODE_A])        {            dv1 += Vector2d::i*-1/phys_dt;        }        if(keyStates[SDL_Scancode::SDL_SCANCODE_UP])        {            dv2 += Vector2d::j/phys_dt;        }        if(keyStates[SDL_Scancode::SDL_SCANCODE_RIGHT])        {            dv2 += Vector2d::i/phys_dt;        }        if(keyStates[SDL_Scancode::SDL_SCANCODE_DOWN])        {            dv2 += Vector2d::j*-1/phys_dt;        }        if(keyStates[SDL_Scancode::SDL_SCANCODE_LEFT])        {            dv2 += Vector2d::i*-1/phys_dt;        }#ifdef ROPE_TEST        dv1/=4;        balls[0]->vel = dv1;#ifdef CONTROL_BOTH_ENDS        dv2/=4;        balls[chain_length - 1]->vel = dv2;#else        dv2*=100;        balls[chain_length - 1]->applyForce(dv2 * balls[chain_length - 1]->mass);#endif        for(int i = 0; i < chain_length; i++)        {            balls[i]->applyForce(balls[i]->vel * balls[i]->vel.magnitude() * -1);        }#endif#ifdef CLOTH_TEST        balls[0][0]->vel = dv1/4;#ifdef CONTROL_BOTH_ENDS        balls[clothWidth-1][0]->vel = balls[0][0]->vel + dv2/4;#endif        for(int i = 0; i < clothWidth; i++)        {            for(int j = 0; j < clothHeight; j++)            {                balls[i][j]->applyForce(balls[i][j]->vel * balls[i][j]->vel.magnitude() * -1);            }        }#endif // CLOTH_TEST        if(keyStates[SDL_Scancode::SDL_SCANCODE_Q])        {            break;        }    }    delete[] balls;#ifdef CIRCULAR_GRAVITY_TEST    delete[] wall;#endif    SDL_Quit();    return 0;}