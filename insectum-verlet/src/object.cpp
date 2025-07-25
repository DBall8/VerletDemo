#include "object.hpp"
#include <iostream>

namespace verlet
{
    Object::Object(float x, float y, float size):
        position_(Vec2(x, y)),
        prevPosition_(position_),
        newVelocity_(Vec2::Zero()),
        acceleration_(Vec2::Zero()),
        radius_(size),
        fixed_(false)
    {}

    Object::Object(Vec2 position, float size):
        position_(position),
        prevPosition_(position_),
        newVelocity_(Vec2::Zero()),
        acceleration_(Vec2::Zero()),
        radius_(size),
        fixed_(false)
    {}

    Object::~Object(){}

    void Object::update(float dt)
    {
        if (fixed_) return;

        if (newVelocity_.x != 0 || newVelocity_.y != 0)
        {
            prevPosition_ = position_ - (newVelocity_ * dt);
            newVelocity_ = Vec2::Zero();
        }

        Vec2 delta = position_ - prevPosition_;
        Vec2 prevPosition = position_;

        position_ = position_ + delta + (acceleration_ * dt * dt);
        prevPosition_ = prevPosition;
        acceleration_ = Vec2::Zero();
    }

    void Object::setVelocity(Vec2 v)
    {
        newVelocity_ = v;
    }

    void Object::addVelocity(Vec2 v)
    {
        newVelocity_ = newVelocity_ + v;
    }

    Vec2 Object::getVelocity()
    {
        return position_ - prevPosition_;
    }
}