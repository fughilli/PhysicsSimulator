#include "physeng.h"
#include "gameeng.h"

GameEngine::GameEngine(int physTicksPerGameTick) : PhysicsEngine(), m_physTicksPerGameTick(physTicksPerGameTick)
{}

void GameEngine::step(double dt)
{
    for(int i = 0; i < m_physTicksPerGameTick; i++)
    {
        stepSim(dt/m_physTicksPerGameTick);
    }

    for(vector<PhysicsObject*>::iterator iter = m_physicsObjects.begin(); iter != m_physicsObjects.end(); iter++)
    {
        GameObject* i_gameObject;
        if((i_gameObject = dynamic_cast<GameObject*>(*iter)))
        {
            i_gameObject->draw();
        }
    }
}

GameObject::GameObject(GameEngine& gameEngine, bool dynamic) : PhysicsObject(gameEngine, dynamic)
{}
