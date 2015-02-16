#ifndef PHYS_CONSTRAINT_H
#define PHYS_CONSTRAINT_H

#include "physeng.h"
#include "VectorLib/Vector.h"

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
    SpringConstraint(PhysicsEngine& physicsEngine, PhysicsObject* physObj1, PhysicsObject* physObj2, double length, double K = 10000, double damp = 50);
    ~SpringConstraint();
    virtual bool resolveConstraint(double dt);
protected:
    double m_length;
    double m_dampingFactor;
    double m_K;
};

class RopeConstraint : public SpringConstraint
{
public:
    RopeConstraint(PhysicsEngine& physicsEngine, PhysicsObject* physObj1, PhysicsObject* physObj2, double length, double K = 10000, double damp = 50);
    ~RopeConstraint();
    virtual bool resolveConstraint(double dt);
};

class SlideConstraint : public PhysicsConstraint
{
public:
    SlideConstraint(PhysicsEngine& physicsEngine, PhysicsObject* physObj, Vector2d direction, Vector2d position, double K = 10000, double linearDamping = 0,
                    double transverseDamping = 50, double extentA = -1, double extentB = -1);
    ~SlideConstraint();
    bool resolveConstraint(double dt);
private:
    Vector2d m_direction, m_position, m_endpointA, m_endpointB;
    double m_K, m_linearDamping, m_transverseDamping;
    double m_extentA, m_extentB;
};

class CircularConstraint : public PhysicsConstraint
{
public:
    CircularConstraint(PhysicsEngine& physicsEngine, PhysicsObject* physObj, Vector2d position, double radius, double K = 10000, double angularDamping = 0, double radialDamping = 0);
    ~CircularConstraint();
    bool resolveConstraint(double dt);
private:
    Vector2d m_position;
    double m_radius, m_K, m_angularDamping, m_radialDamping;
};

#endif // PHYS_CONSTRAINT_H
