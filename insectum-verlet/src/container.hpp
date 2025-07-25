#pragma once

#include <vector>
#include "object.hpp"
#include "cell_grid.hpp"
#include "thread_pool.hpp"

namespace verlet
{
    struct Constraint
    {
        Vec2 position;
        float width;
        float height;
        bool isCircle;
    };

    class Container
    {
        public:
            Container();
            ~Container();

            void setGravity(float gravity){ gravity_ = gravity; }
            void setUpdateRate(float rate);
            void setBounciness(float bounciness){ bounciness_ = bounciness; }
            void setPhysicsSubsteps(int substeps);
            void setCircleConstraint(float centerX, float centerY, float radius);
            void setRectConstraint(float centerX, float centerY, float width, float height);

            std::vector<Object> getObjects() { return objects_; }
            int getNumObjects(){ return objects_.size(); }
            Constraint getConstraint(){ return constraint_; }

            void addObject(Object o);
            void addFixedObject(Object o);
            void update();

        private:
            std::vector<Object> objects_;
            Constraint constraint_ = Constraint(Vec2::Zero(), 100, 100, false);
            ThreadPool threadPool_;
            float gravity_ = 0;
            float timeStep_ = 1.0f / 60.0f;
            float bounciness_ = 0.90f;
            int physicsSubsteps_ = 8;
            float subStepTime_ = timeStep_ / physicsSubsteps_;

            float cellSize_ = 1.0f;

            CellGrid cellGrid_;

            void applyGravity();
            void addObjectsToGrid();
            void checkCollisions();
            void checkCollisionsObject(Object* pObject, Cell* pCell);
            void checkCollisionsCell(int i, int j);
            void checkCollisionsRegion(int startX, int endX, int startY, int endY);
            void checkCircleConstraint(Object* pObject);
            void checkRectConstraint(Object* pObject);
            void checkConstraints();
            void updateObjects(float dt);
    };
}