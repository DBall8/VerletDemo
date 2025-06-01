#include "utils.hpp"
#include <cmath>

namespace verlet
{
    Vec2::Vec2(float x, float y):
        x(x),
        y(y)
    {
    }

    Vec2::~Vec2(){}

    Vec2 Vec2::operator+(Vec2 other)
    {
        return Vec2(x + other.x, y + other.y);
    }

    Vec2 Vec2::operator-(Vec2 other)
    {
        return Vec2(x - other.x, y - other.y);
    }

    Vec2 Vec2::operator*(float scalar)
    {
        return Vec2(x * scalar, y * scalar);
    }

    Vec2 Vec2::operator/(float scalar)
    {
        if (scalar == 0) return Vec2(0,0);
        return Vec2(x / scalar, y / scalar);
    }

    float Vec2::distance(Vec2 other)
    {
        return sqrt(distanceSquared(other));
    }

    float Vec2::distanceSquared(Vec2 other)
    {
        float dx = x - other.x;
        float dy = y - other.y;
        return (dx * dx) + (dy * dy);
    }
}