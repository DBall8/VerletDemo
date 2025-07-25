#pragma once

#include "utils.hpp"

namespace verlet
{
    class Object
    {
        public:
            Object(float x, float y, float size);
            Object(Vec2 position, float size);
            ~Object();
            float getRadius() { return radius_; }
            Vec2 getPosition() { return position_; }
            Vec2 getVelocity();
            bool isFixed(){ return fixed_; }

            void setPosition(Vec2 newPosition) { position_ = newPosition; }
            void setVelocity(Vec2 v);
            void addVelocity(Vec2 v);
            void addAcceleration(Vec2 a) { acceleration_ = acceleration_ + a; }
            void setAcceleration(Vec2 a) { acceleration_ = a; }
            void setFixed(bool fixed) { fixed_ = fixed; }

            void update(float dt);

        private:
            float radius_;
            Vec2 position_;
            Vec2 prevPosition_;
            Vec2 newVelocity_;
            Vec2 acceleration_;
            bool fixed_ = false;
    };
}
