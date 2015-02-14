#ifndef SHAPE_SOLVER_H
#define SHAPE_SOLVER_H

#include "Vector/Vector.h"
#include <vector>

bool lineSegIntersect(Vector2d a1, Vector2d a2, Vector2d b1, Vector2d b2, Vector2d& res);

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

#endif // SHAPE_SOLVER_H
