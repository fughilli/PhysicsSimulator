#ifndef GAMEENG_H
#define GAMEENG_H

#include "physeng.h"

class GameEngine;
class GameObject;

extern void draw();

class GameEngine : public PhysicsEngine
{
public:
    GameEngine(int physTicksPerGameTick);
    void step(double dt);
private:
    int m_physTicksPerGameTick;
    friend GameObject;
};

class GameObject : public PhysicsObject
{
public:
    GameObject(GameEngine& gameEngine, bool dynamic = true);
private:
    virtual void draw() = 0;
    friend GameEngine;
};

#endif // GAMEENG_H
