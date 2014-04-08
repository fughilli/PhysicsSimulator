#include "physeng.h"
#include "Vector/Vector.h"
#include <vector>

// METERS, KILOGRAMS, SECONDS

PhysicsEngine::PhysicsEngine()
{
    G = 6.67e-11;
}

PhysicsEngine::~PhysicsEngine()
{

}

void PhysicsEngine::stepSim(double dt)
{
    for(std::vector<PhysicsObject*>::iterator iter = m_physicsObjects.begin(); iter != m_physicsObjects.end(); iter++)
    {

        for(std::vector<PhysicsObject*>::iterator iter_inner = m_physicsObjects.begin(); iter_inner != m_physicsObjects.end(); iter_inner++)
        {
            // Objects don't affect themselves...
            if(iter_inner == iter)
                continue;

            // Simulate forces...

            // From force-fields
            ForceField* i_forceField;
            if((i_forceField = dynamic_cast<ForceField*>(*iter)))
            {
                if((*iter_inner)->dynamic)
                {
                    (*iter_inner)->force += i_forceField->fieldForce;
                    (*iter_inner)->vel += i_forceField->fieldAccel*dt;
                }
            }
            else
            {
                // From gravity of supermassive objects
                if((*iter_inner)->selfGravity)
                {
                    Vector2d gForce = ((*iter_inner)->pos - (*iter)->pos);
                    double R = gForce.magnitude();
                    gForce = gForce.unit()*G*((*iter_inner)->mass)*((*iter)->mass);
                    gForce /= (R*R);
                    (*iter)->force += gForce;
                }

                // Simulate collisions
                if((*iter_inner)->dynamic && !(*iter_inner)->noCollide && !(*iter)->noCollide)
                {
                    Vector2d hitNormal;
                    if((*iter)->checkCollision(*(*iter_inner), hitNormal))
                    {
                        if((*iter)->dynamic)
                        {
                            (*iter_inner)->impulse += (hitNormal.unit() *
                                                       abs(
                                                           hitNormal.unit().dot(
                                                               ((*iter)->vel*(*iter)->mass)-((*iter_inner)->vel*(*iter_inner)->mass))
                                                       )
                                                      );
                        }
                        else
                        {
                            (*iter_inner)->impulse -= ((*iter_inner)->vel * (*iter_inner)->mass).project(hitNormal)*2;
                        }
                    }
                }
            }
        }
    }

    for(std::vector<PhysicsConstraint*>::iterator iter = m_constraints.begin(); iter != m_constraints.end(); iter++)
    {
        (*iter)->resolveConstraint(dt);
    }

    for(std::vector<PhysicsObject*>::iterator iter = m_physicsObjects.begin(); iter != m_physicsObjects.end(); iter++)
    {
        // Compute resultant velocities and displacements
        if((*iter)->dynamic)
        {
            (*iter)->vel += ((*iter)->impulse + ((*iter)->force * dt))/(*iter)->mass;
            (*iter)->force = Vector2d::zero;
            (*iter)->impulse = Vector2d::zero;
            (*iter)->pos += (*iter)->vel*dt;
            (*iter)->angle += (*iter)->angvel;
        }
        else
        {
            (*iter)->pos += (*iter)->vel*dt;
            (*iter)->angle += (*iter)->angvel;
        }
    }
}

PhysicsObject::PhysicsObject(PhysicsEngine& physicsEngine, bool dynamic) : dynamic(dynamic)
{
    m_physicsEngine = &physicsEngine;
    m_physicsEngine->m_physicsObjects.push_back(this);
    mass = 1;
    angle = angvel = 0;
    pos = vel = Vector2d::zero;
    selfGravity = false;    noCollide = false;
}

PhysicsObject::~PhysicsObject()
{
    // Invalidate the physics object smart pointer
    std::vector<PhysicsObject*>::iterator iter = m_physicsEngine->m_physicsObjects.begin();
    while(iter != m_physicsEngine->m_physicsObjects.end())
    {
        if(*iter == this)
        {
            iter = m_physicsEngine->m_physicsObjects.erase(iter);
            break;
        }
        else
        {
            iter++;
        }
    }
}

void PhysicsObject::applyImpulse(Vector2d imp)
{
    impulse += imp;
}

void PhysicsObject::applyForce(Vector2d fce)
{
    force += fce;
}

// Returns hit normal in direction of other object, with a magnitude equivalent to the depth of intersection
bool PhysicsObject::checkCollision(const PhysicsObject& other, Vector2d& hitNormal) const
{
    if((other.pos - pos).magnitude() < (radius + other.radius))
    {
        hitNormal = other.pos - pos;
        double dis = hitNormal.magnitude();
        hitNormal = hitNormal.unit();
        hitNormal *= abs((radius + other.radius)-dis);
        return true;
    }
    return false;
}

//Polygon::intersect

//BoundingBox::intersect(const BoundingBox& other) const
