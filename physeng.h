#ifndef PHYSENG_H
#define PHYSENG_H

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
    static bool lineSegIntersect(Vector2d a1, Vector2d a2, Vector2d b1, Vector2d b2, Vector2d& res);
    virtual void debugDrawVec(const Vector2d& origin, const Vector2d& vec, uint8_t r, uint8_t g, uint8_t b) const = 0;
protected:
    std::vector<PhysicsObject*> m_physicsObjects;
    std::vector<PhysicsConstraint*> m_constraints;
    friend PhysicsObject;
    friend PhysicsConstraint;
};

struct BoundingBox
{
public:
    Vector2d pos,a,b;
    BoundingBox();
    bool intersect(const BoundingBox& other) const;
};

struct Polygon
{
public:
    Vector2d pos;
    std::vector<Vector2d> points;
    bool intersect(const Polygon& other, Vector2d& hitNormal) const;
    Polygon();
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

#endif // PHYSENG_H
