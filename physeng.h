#ifndef PHYSENG_H
#define PHYSENG_H

#include "physeng_constraint.h"

#include "Vector/Vector.h"
#include <vector>

class PhysicsObject;
class PhysicsConstraint;

class PhysicsEngine
{
public:
    double G;
    void stepSim(double dt);
    PhysicsEngine();
    virtual ~PhysicsEngine();
    virtual void debugDrawVec(const Vector2d& origin, const Vector2d& vec, uint8_t r, uint8_t g, uint8_t b) const = 0;
protected:
    std::vector<PhysicsObject*> m_physicsObjects;
    std::vector<PhysicsConstraint*> m_constraints;
    friend PhysicsObject;
    friend PhysicsConstraint;
};

class PhysicsObject
{
public:
    bool selfGravity;
    double mass;
    double radius;
    Vector2d pos;
    Vector2d vel;
    double angle;
    double angvel;
    bool dynamic;    bool noCollide;
    PhysicsObject(PhysicsEngine& physicsEngine, bool dynamic = true);
    virtual ~PhysicsObject();
    void applyImpulse(Vector2d imp);
    void applyForce(Vector2d fce);
    virtual bool checkCollision(const PhysicsObject& other, Vector2d& hitNormal) const;
private:

    PhysicsEngine *m_physicsEngine;
    Vector2d impulse;
    Vector2d force;
    friend PhysicsEngine;
};

class ForceField : public PhysicsObject
{
public:
    Vector2d fieldForce;
    Vector2d fieldAccel;
    ForceField(PhysicsEngine& physicsEngine) : PhysicsObject(physicsEngine, false) {}
    ~ForceField() {}
};

#endif // PHYSENG_H
