#pragma once

#include <vector>
#include "object.hpp"
#include "cell_grid.hpp"

namespace verlet
{
    class Container
    {
        public:
            Container();
            ~Container();

            void setGravity(float gravity){ gravity_ = gravity; }
            void setUpdateRate(float rate){ timeStep_ = 1.0f / rate; }
            void setBounciness(float bounciness){ bounciness_ = bounciness; }
            void setPhysicsSubsteps(int substeps) { physicsSubsteps_ = substeps; }
            void setConstraint(float centerX, float centerY, float radius){ constraintCenter_ = Vec2(centerX, centerY); constraintRadius_ = radius; }

            float getConstraintRadius(){ return constraintRadius_; }
            Vec2 getConstraintCenter(){ return constraintCenter_; }
            std::vector<Object> getObjects() { return objects_; }
            int getNumObjects(){ return objects_.size(); }

            void addObject(Object o);
            void update();

        private:
            std::vector<Object> objects_; 
            float gravity_ = 0;
            float timeStep_ = 1.0f / 60.0f;
            float bounciness_ = 0.90f;
            int physicsSubsteps_ = 8;

            float cellSize_ = 10.0f;

            float constraintRadius_ = 1000.0f;
            Vec2 constraintCenter_ = Vec2::Zero();

            CellGrid cellGrid_;

            void applyGravity();
            void addObjectsToGrid();
            void checkCollisions();
            void checkCollisionsObject(Object* pObject, std::vector<Object*> objects);
            void checkCollisionsCell(int i, int j);
            void checkConstraints();
            void updateObjects(float dt);
    };
}