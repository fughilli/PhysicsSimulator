#ifndef PHYS_CONSTRAINT_H
#define PHYS_CONSTRAINT_H

#include "physeng.h"

#include <vector>

class PhysicsEngine;
class PhysicsObject;

class PhysicsConstraint
{
public:
    PhysicsConstraint(PhysicsEngine& physicsEngine);
    virtual ~PhysicsConstraint();
    virtual bool resolveConstraint(double dt) = 0;
protected:
    std::vector<PhysicsObject*> m_constrainedObjects;
    PhysicsEngine *m_physicsEngine;
    friend PhysicsEngine;
};

class SpringConstraint : public PhysicsConstraint
{
public:
    SpringConstraint(PhysicsEngine& physicsEngine, PhysicsObject* physObj1, PhysicsObject* physObj2, double length, double K, double damp);
    ~SpringConstraint();
    bool resolveConstraint(double dt);
    double m_length;
    double m_dampingFactor;
    double m_K;
};

class RopeConstraint : public PhysicsConstraint
{
public:
    RopeConstraint(PhysicsEngine& physicsEngine, PhysicsObject* physObj1, PhysicsObject* physObj2, double length);
    ~RopeConstraint();
    bool resolveConstraint(double dt);
    double m_length;
    double m_dampingFactor;
    double m_K;
};

#endif // PHYS_CONSTRAINT_H
