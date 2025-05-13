#include "object.hpp"
#include <iostream>

namespace verlet
{
    Object::Object(float x, float y, float size):
        position_(Vec2(x, y)),
        prevPosition_(position_),
        acceleration_(Vec2::Zero()),
        radius_(size)
    {}

    Object::Object(Vec2 position, float size):
        position_(position),
        prevPosition_(position_),
        acceleration_(Vec2::Zero()),
        radius_(size)
    {}

    Object::~Object(){}

    void Object::update(float dt)
    {
        Vec2 delta = position_ - prevPosition_;
        Vec2 prevPosition = position_;

        position_ = position_ + delta + (acceleration_ * dt * dt);
        prevPosition_ = prevPosition;
        acceleration_ = Vec2::Zero();
    }

    void Object::setVelocity(Vec2 v)
    {
        prevPosition_ = position_ - v;
    }

    Vec2 Object::getVelocity()
    {
        return position_ - prevPosition_;
    }
}