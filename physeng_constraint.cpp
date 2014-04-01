#include "physeng.h"
#include "sdlgameeng.h"
#include <iostream>

#define DEFAULT_DAMPING_FACTOR (100.0)
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
//    bool ret = false;
//    Vector2d ropeVec = m_constrainedObjects[0]->pos -
//                       m_constrainedObjects[1]->pos;
//    if(ropeVec.magnitude() > m_length)
//    {
//        m_constrainedObjects[1]->vel -=
//            m_constrainedObjects[1]->vel.project(ropeVec)*m_dampingFactor;
//        m_constrainedObjects[0]->vel +=
//            m_constrainedObjects[0]->vel.project(ropeVec)*m_dampingFactor;
//        ret = true;
//    }
//    if(ropeVec.magnitude() > (m_length*IMPULSE_THRESHOLD))
//    {
//        double ropeTension =
//            (((m_constrainedObjects[0]->vel * m_constrainedObjects[0]->mass) -
//              (m_constrainedObjects[1]->vel * m_constrainedObjects[1]->mass))/dt).magnitude();
//        m_constrainedObjects[1]->applyForce(
//            (m_constrainedObjects[0]->pos - m_constrainedObjects[1]->pos).unit()*ropeTension);
//        m_constrainedObjects[0]->applyForce(
//            (m_constrainedObjects[1]->pos - m_constrainedObjects[0]->pos).unit()*ropeTension);
//        ret = true;
//    }
//    return ret;
    bool ret = false;
    Vector2d ropeVec = (m_constrainedObjects[0]->pos -
                        m_constrainedObjects[1]->pos);

//    Vector2d COM =
//    (m_constrainedObjects[0]->pos * m_constrainedObjects[0]->mass +
//     m_constrainedObjects[1]->pos * m_constrainedObjects[1]->mass)/
//     (m_constrainedObjects[0]->mass * m_constrainedObjects[1]->mass);
//    Vector2d COMVel =
//        (m_constrainedObjects[0]->vel * m_constrainedObjects[0]->mass +
//         m_constrainedObjects[1]->vel * m_constrainedObjects[1]->mass)/
//        (m_constrainedObjects[0]->mass + m_constrainedObjects[1]->mass);
    Vector2d CVel =
        (m_constrainedObjects[0]->vel + m_constrainedObjects[1]->vel)/2;
#ifdef DEBUGDRAW
    m_physicsEngine->debugDrawVec((m_constrainedObjects[0]->pos + m_constrainedObjects[1]->pos)/2, ropeVec.unit(), 128, 128, 255);

    m_physicsEngine->debugDrawVec(m_constrainedObjects[0]->pos, m_constrainedObjects[0]->vel, 255, 128, 128);
    m_physicsEngine->debugDrawVec(m_constrainedObjects[1]->pos, m_constrainedObjects[1]->vel, 255, 255, 128);
#endif

//    if(ropeVec.magnitude() > m_length)
//    {
//        m_constrainedObjects[1]->applyImpulse(
//            m_constrainedObjects[1]->vel.project(ropeVec)*-m_dampingFactor*m_constrainedObjects[1]->mass);
//        m_constrainedObjects[0]->applyImpulse(
//            m_constrainedObjects[0]->vel.project(ropeVec)*m_dampingFactor*m_constrainedObjects[0]->mass);
//        ret = true;
//    }
    double extension = ropeVec.magnitude() - m_length;

    if(extension > 0)
    {

//        Vector2d ropeTension =
//            ((m_constrainedObjects[0]->vel * m_constrainedObjects[0]->mass) -
//             (m_constrainedObjects[1]->vel * m_constrainedObjects[1]->mass)).project(ropeVec) / dt;

//        if(m_constrainedObjects[1]->vel.dot(ropeVec) <= 0)
//            m_constrainedObjects[1]->applyForce(ropeTension);
//        if(m_constrainedObjects[0]->vel.dot(ropeVec) >= 0)
//            m_constrainedObjects[0]->applyForce((ropeTension*-1));

        Vector2d unitRopeVec = ropeVec.unit();
        double springForce = m_K * extension;

//        double obj0Damping = (m_constrainedObjects[0]->vel*m_constrainedObjects[0]->mass - COMVel).dot(unitRopeVec) * m_dampingFactor;
//        double obj1Damping = (m_constrainedObjects[1]->vel*m_constrainedObjects[1]->mass - COMVel).dot(unitRopeVec) * m_dampingFactor;

        double obj0Damping = (m_constrainedObjects[0]->vel - m_constrainedObjects[1]->vel).dot(unitRopeVec) * m_dampingFactor;
        double obj1Damping = (m_constrainedObjects[1]->vel - m_constrainedObjects[0]->vel).dot(unitRopeVec) * m_dampingFactor;

        double obj0Force = -springForce;
        double obj1Force = springForce;

        obj0Force -= obj0Damping;
        obj1Force -= obj1Damping;

        obj0Force *= m_constrainedObjects[0]->mass;
        obj1Force *= m_constrainedObjects[1]->mass;

//        if(m_constrainedObjects[0]->vel.dot(unitRopeVec) > 0)
//            m_constrainedObjects[0]->applyImpulse(m_constrainedObjects[0]->vel.project(unitRopeVec)*-m_constrainedObjects[0]->mass*2);
//        if(m_constrainedObjects[1]->vel.dot(unitRopeVec) < 0)
//            m_constrainedObjects[1]->applyImpulse(m_constrainedObjects[1]->vel.project(unitRopeVec)*-m_constrainedObjects[1]->mass*2);

        m_constrainedObjects[0]->applyForce(unitRopeVec * (obj0Force));
        m_constrainedObjects[1]->applyForce(unitRopeVec * (obj1Force));

//        m_physicsEngine->debugDrawVec(
//            m_constrainedObjects[0]->pos, unitRopeVec * obj0Force, 255, 128, 128);
//        m_physicsEngine->debugDrawVec(
//            m_constrainedObjects[1]->pos, unitRopeVec * obj1Force, 255, 255, 128);

        ret = true;
    }
    return ret;
}
