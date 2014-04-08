#include "physeng.h"
#include "sdlgameeng.h"
#include <iostream>

#define DEFAULT_DAMPING_FACTOR (50.0)
#define IMPULSE_THRESHOLD (1)

//#define DEBUGDRAW

PhysicsConstraint::PhysicsConstraint(PhysicsEngine& physicsEngine)
{
    m_physicsEngine = &physicsEngine;
    m_physicsEngine->m_constraints.push_back(this);
}

PhysicsConstraint::~PhysicsConstraint()
{
    std::vector<PhysicsConstraint*>::iterator iter = m_physicsEngine->m_constraints.begin();
    while(iter != m_physicsEngine->m_constraints.end())
    {
        if(*iter == this)
        {
            iter = m_physicsEngine->m_constraints.erase(iter);
            break;
        }
        else
        {
            iter++;
        }
    }
}

RopeConstraint::RopeConstraint(PhysicsEngine& physicsEngine, PhysicsObject* physObj1, PhysicsObject* physObj2, double length) :
    PhysicsConstraint(physicsEngine)
{
    m_constrainedObjects.push_back(physObj1);
    m_constrainedObjects.push_back(physObj2);
    m_length = length;
    m_dampingFactor = DEFAULT_DAMPING_FACTOR;
    m_K = 10000;
}

RopeConstraint::~RopeConstraint()
{

}

bool RopeConstraint::resolveConstraint(double dt)
{
    bool ret = false;
    Vector2d ropeVec = (m_constrainedObjects[0]->pos -
                        m_constrainedObjects[1]->pos);

    Vector2d CVel =
        (m_constrainedObjects[0]->vel + m_constrainedObjects[1]->vel)/2;
#ifdef DEBUGDRAW
    m_physicsEngine->debugDrawVec((m_constrainedObjects[0]->pos + m_constrainedObjects[1]->pos)/2, ropeVec.unit(), 128, 128, 255);

    m_physicsEngine->debugDrawVec(m_constrainedObjects[0]->pos, m_constrainedObjects[0]->vel, 255, 128, 128);
    m_physicsEngine->debugDrawVec(m_constrainedObjects[1]->pos, m_constrainedObjects[1]->vel, 255, 255, 128);
#endif
    double extension = ropeVec.magnitude() - m_length;

    if(extension > 0)
    {
        Vector2d unitRopeVec = ropeVec.unit();
        double springForce = m_K * extension;

        double obj0Damping = (m_constrainedObjects[0]->vel - m_constrainedObjects[1]->vel).dot(unitRopeVec) * m_dampingFactor;
        double obj1Damping = (m_constrainedObjects[1]->vel - m_constrainedObjects[0]->vel).dot(unitRopeVec) * m_dampingFactor;

        double obj0Force = -springForce;
        double obj1Force = springForce;

        obj0Force -= obj0Damping;
        obj1Force -= obj1Damping;

        //obj0Force *= m_constrainedObjects[0]->mass;
        //obj1Force *= m_constrainedObjects[1]->mass;

        m_constrainedObjects[0]->applyForce(unitRopeVec * (obj0Force));
        m_constrainedObjects[1]->applyForce(unitRopeVec * (obj1Force));

        ret = true;
    }
    return ret;
}

SpringConstraint::SpringConstraint(PhysicsEngine& physicsEngine, PhysicsObject* physObj1, PhysicsObject* physObj2, double length,
                                   double K, double damp) :
    PhysicsConstraint(physicsEngine)
{
    m_constrainedObjects.push_back(physObj1);
    m_constrainedObjects.push_back(physObj2);
    m_length = length;
    m_dampingFactor = damp;
    m_K = K;
}

SpringConstraint::~SpringConstraint()
{

}

bool SpringConstraint::resolveConstraint(double dt)
{
    bool ret = false;
    Vector2d ropeVec = (m_constrainedObjects[0]->pos -
                        m_constrainedObjects[1]->pos);

    Vector2d CVel =
        (m_constrainedObjects[0]->vel + m_constrainedObjects[1]->vel)/2;
#ifdef DEBUGDRAW
    m_physicsEngine->debugDrawVec((m_constrainedObjects[0]->pos + m_constrainedObjects[1]->pos)/2, ropeVec.unit(), 128, 128, 255);

    m_physicsEngine->debugDrawVec(m_constrainedObjects[0]->pos, m_constrainedObjects[0]->vel, 255, 128, 128);
    m_physicsEngine->debugDrawVec(m_constrainedObjects[1]->pos, m_constrainedObjects[1]->vel, 255, 255, 128);
#endif
    double extension = ropeVec.magnitude() - m_length;

    Vector2d unitRopeVec = ropeVec.unit();
    double springForce = m_K * extension;

    double obj0Damping = (m_constrainedObjects[0]->vel - m_constrainedObjects[1]->vel).dot(unitRopeVec) * m_dampingFactor;
    double obj1Damping = (m_constrainedObjects[1]->vel - m_constrainedObjects[0]->vel).dot(unitRopeVec) * m_dampingFactor;

    double obj0Force = -springForce;
    double obj1Force = springForce;

    obj0Force -= obj0Damping;
    obj1Force -= obj1Damping;

    //obj0Force *= m_constrainedObjects[0]->mass;
    //obj1Force *= m_constrainedObjects[1]->mass;

    m_constrainedObjects[0]->applyForce(unitRopeVec * (obj0Force));
    m_constrainedObjects[1]->applyForce(unitRopeVec * (obj1Force));

    return true;
}
