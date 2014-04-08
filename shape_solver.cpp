#include "shape_solver.h"
#include "SDL2/SDL.h"

bool lineSegIntersect(Vector2d a1, Vector2d a2, Vector2d b1, Vector2d b2, Vector2d& res)
{
    // TODO: implement line segment intersection check w/last argument modified to be intersection point (if any)
    return false;
}

Polygon::Polygon()
{

}

BoundingBox::BoundingBox()
{
    a = Vector2d::zero;
    b = Vector2d::zero;
}

bool BoundingBox::intersect(const BoundingBox& other) const
{

}

bool Polygon::intersect(const Polygon& other, Vector2d& hitNormal) const
{

}
