#include "container.hpp"
#include <iostream>

namespace verlet
{
    Container::Container()
    {}
    Container::~Container(){}

    void Container::addObject(Object o)
    {
        objects_.push_back(o);

        if (o.getRadius() > cellSize_)
        {
            cellSize_ = o.getRadius();
        }
    }

    void Container::applyGravity()
    {
        float gravityScaled = gravity_ / timeStep_;
        for (Object& o: objects_)
        {
            o.addAcceleration(Vec2(0, gravityScaled));
        }
    }

    void Container::addObjectsToGrid()
    {
        cellGrid_.clear();
        cellGrid_.resize(cellSize_, constraintRadius_*2.0f, constraintRadius_*2.0f);
        for (Object& o: objects_)
        {
            cellGrid_.addObject(&o);
        }
    }

    void Container::checkCollisions()
    {
        addObjectsToGrid();
        for (int j=0; j<cellGrid_.getHeight(); j++)
        {
            for (int i=0; i<cellGrid_.getWidth(); i++)
            {
                checkCollisionsCell(i, j);
            }
        }
    }

    void Container::checkCollisionsCell(int cellX, int cellY)
    {
        Cell* pCell = cellGrid_.getCell(cellX, cellY);
        if (pCell == nullptr) return;
        
        while (pCell->getNumObjects() > 0)
        {
            Object* pObject = pCell->removeObject();
            if (pObject == nullptr) return;

            int minX = cellX - 2;
            int maxX = cellX + 2;
            if (minX < 0) minX = 0;
            if (maxX >= cellGrid_.getWidth()) maxX = cellGrid_.getWidth() - 1;

            int minY = cellY - 2;
            int maxY = cellY + 2;
            if (minY < 0) minY = 0;
            if (maxY >= cellGrid_.getHeight()) maxY = cellGrid_.getHeight() - 1;

            for (int j=minY; j<=maxY; j++)
            {
                for (int i=minX; i<=maxX; i++)
                {
                    checkCollisionsObject(pObject, cellGrid_.getCell(i, j)->getObjects());
                }
            }

        }
    }

    void Container::checkCollisionsObject(Object* pObject, std::vector<Object*> objects)
    {
        for (Object* pOtherObject: objects)
        {
            float totalRadius = pObject->getRadius() + pOtherObject->getRadius();        // If they are closer than this distance, they're overlapping
            Vec2 overlapVector = pOtherObject->getPosition() -pObject->getPosition();   // Relative positions of the centers

            float distanceSquared = overlapVector.distanceSquared(Vec2::Zero());
            float totalRadiusSquared = totalRadius * totalRadius;

            if (totalRadiusSquared > distanceSquared)
            {
                float distance = sqrt(distanceSquared);      // Scalar distance between centers
                float overlap = totalRadius - distance;      // Amount of overlap (positive = overlap, negative = space between)

                Vec2 overlapUnit = overlapVector / distance;                                                    // Unit vector of their relative positions
                float ratio1 = pOtherObject->getRadius() / (pObject->getRadius() + pOtherObject->getRadius());  // Mass ratio for object 1
                float ratio2 = pObject->getRadius()      / (pObject->getRadius() + pOtherObject->getRadius());  // Mass ratio for object 2
                Vec2 reaction = overlapUnit * overlap * 0.5f * bounciness_;                                     // Reaction force, directional

                pObject->setPosition(pObject->getPosition() - (reaction * ratio1));
                pOtherObject->setPosition(pOtherObject->getPosition() + (reaction * ratio2));
            }
        }
    }

    void Container::checkConstraints()
    {
        for (Object& o: objects_)
        {
            Vec2 positionRelative = o.getPosition() - constraintCenter_;    // Position of object relative to constraint center
            float distance = sqrt((positionRelative.x * positionRelative.x) + (positionRelative.y * positionRelative.y));   // Distance from constraint center

            if (distance > constraintRadius_ - o.getRadius())
            {
                // Move out by exactly the overshoot
                Vec2 newPosition = (positionRelative / distance) * (constraintRadius_ - o.getRadius());
                newPosition = newPosition + constraintCenter_;
                o.setPosition(newPosition);
            }
        }
    }

    void Container::updateObjects(float dt)
    {
        for (Object& o: objects_)
        {
            o.update(dt);
        }
    }

    void Container::update()
    {

        float dt = timeStep_ / physicsSubsteps_;
        for (float time = dt; time <= timeStep_; time += dt)
        {
            applyGravity();
            checkCollisions();
            checkConstraints();
            updateObjects(dt);
        }
    }
}