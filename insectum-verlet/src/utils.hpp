#pragma once

namespace verlet
{
    class Vec2
    {
        public:
            float x;
            float y;

            Vec2(float x, float y);
            ~Vec2();

            static Vec2 Zero() { return Vec2(0,0); }
            static Vec2 Unit() { return Vec2(1,1); }

            void operator=(Vec2 other) { x = other.x; y = other.y; }
            Vec2 operator+(Vec2 other);
            Vec2 operator-(Vec2 other);
            Vec2 operator*(float scalar);
            Vec2 operator/(float scalar);

            float distance(Vec2 other);
            float distanceSquared(Vec2 other);

            char* toString();
    };
}