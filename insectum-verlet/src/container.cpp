#include "container.hpp"
#include <iostream>
#include <thread>
#include "timing.hpp"

// static void testNum()
// {
//     for (int i=0; i<100; i++)
//     {
//         std::cout << i << "\n";
//     }
// }

// static void testLet()
// {
//     for (int i=0; i<100; i++)
//     {
//         std::cout << (char)('a' + (i % 23)) << "\n";
//     }
// }

const int NUM_THREAD = 4;

static int numChecks = 0;
static int maxChecks = 0;
static int minChecks = 99000;
static bool doOnce = false;

namespace verlet
{
    Container::Container():
        threadPool_(ThreadPool(NUM_THREAD))
    {
        // std::function<void()> testNumFunc = testNum;
        // std::function<void()> testLetFunc = testLet;
        // std::cout << "START\n";
        // ThreadPool pool(2);
        // pool.dispatch(testNumFunc);
        // pool.dispatch(testLetFunc);
        // std::cout << "JOIN\n";
        // pool.joinAll();
        // std::cout << "DONE\n";
    }

    Container::~Container(){}

    void Container::setCircleConstraint(float centerX, float centerY, float radius)
    {
        constraint_.position = Vec2(centerX, centerY);
        constraint_.width = radius * 2.0f;
        constraint_.height = radius * 2.0f;
        constraint_.isCircle = true;
        cellGrid_.resize(cellSize_, constraint_.width, constraint_.height);
    }

    void Container::setRectConstraint(float centerX, float centerY, float width, float height)
    {
        constraint_.position = Vec2(centerX - width/2.0f, centerY - height/2.0f);
        constraint_.width = width;
        constraint_.height = height;
        constraint_.isCircle = false;
        cellGrid_.resize(cellSize_, constraint_.width, constraint_.height);
    }

    void Container::addObject(Object o)
    {
        objects_.push_back(o);

        if (o.getRadius() * 2.0f > cellSize_)
        {
            cellSize_ = o.getRadius() * 2.0f;
            cellGrid_.resize(cellSize_, constraint_.width, constraint_.height);
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
        for (Object& o: objects_)
        {
            cellGrid_.addObject(&o);
        }
    }

    void Container::checkCollisions()
    {
        numChecks = 0;

        // startTiming();
        addObjectsToGrid();

        numChecks = 0;
        maxChecks = 0;
        minChecks = 99000;

        // static bool doOnce = true;
        // if (getNumObjects() >= 400 && doOnce)
        // {
        //     for (int j=0; j<cellGrid_.getHeight(); j++)
        //     {
        //         for (int i=0; i<cellGrid_.getWidth(); i++)
        //         {
        //             std::cout << cellGrid_.getCell(i, j)->getNumObjects() << " ";
        //         }
        //         std::cout << std::endl;
        //     }
        //     doOnce = false;
        // }

        // std::cout << "g: " << endTiming() * physicsSubsteps_ << std::endl;
        
        const int portionHeight = cellGrid_.getHeight() / (NUM_THREAD * 2);
        const int portionWidth = cellGrid_.getWidth() / (NUM_THREAD * 2);

        startTiming();
        // checkCollisionsRegion(0, cellGrid_.getWidth(), 0, cellGrid_.getHeight());

        for (int i=0; i<cellGrid_.getHeight(); i+=portionHeight*2)
        {
            int yStart = i;
            int yEnd = i+portionHeight;
            if (yEnd > cellGrid_.getHeight()) yEnd = cellGrid_.getHeight();
            std::function<void()> task = [this, yStart, yEnd](){checkCollisionsRegion(0, cellGrid_.getWidth(), yStart, yEnd);};
            // int xStart = i;
            // int xEnd = i+portionWidth;
            // std::function<void()> task = [this, xStart, xEnd](){checkCollisionsRegion(xStart, xEnd, 0, cellGrid_.getHeight());};
            threadPool_.dispatch(task);
        }

        threadPool_.joinAll();

        for (int i=portionHeight; i<cellGrid_.getHeight(); i+=portionHeight*2)
        {
            int yStart = i;
            int yEnd = i+portionHeight;
            if (yEnd > cellGrid_.getHeight()) yEnd = cellGrid_.getHeight();
            std::function<void()> task = [this, yStart, yEnd](){checkCollisionsRegion(0, cellGrid_.getWidth(), yStart, yEnd);};
            // int xStart = i;
            // int xEnd = i+portionWidth;
            // std::function<void()> task = [this, xStart, xEnd](){checkCollisionsRegion(xStart, xEnd, 0, cellGrid_.getHeight());};            
            threadPool_.dispatch(task);
        }

        threadPool_.joinAll();

        // for (int i=0; i<cellGrid_.getWidth(); i+=2)
        // {
        //     // int yStart = i;
        //     // int yEnd = i+1;
        //     // std::function<void()> task = [this, yStart, yEnd](){checkCollisionsRegion(0, cellGrid_.getWidth(), yStart, yEnd);};
        //     int xStart = i;
        //     int xEnd = i+1;
        //     std::function<void()> task = [this, xStart, xEnd](){checkCollisionsRegion(xStart, xEnd, 0, cellGrid_.getHeight());};
        //     threadPool_.dispatch(task);
        // }

        // threadPool_.joinAll();

        // for (int i=1; i<cellGrid_.getWidth(); i+=2)
        // {
        //     // int yStart = i;
        //     // int yEnd = i+1;
        //     // std::function<void()> task = [this, yStart, yEnd](){checkCollisionsRegion(0, cellGrid_.getWidth(), yStart, yEnd);};
        //     int xStart = i;
        //     int xEnd = i+1;
        //     std::function<void()> task = [this, xStart, xEnd](){checkCollisionsRegion(xStart, xEnd, 0, cellGrid_.getHeight());};            
        //     threadPool_.dispatch(task);
        // }
        
        // threadPool_.joinAll();

        endTiming();

        // float avgChecks = (float)numChecks / (float)getNumObjects();
        // std::cout << avgChecks << " [" << minChecks << "," << maxChecks << "]\n";
    }

    void Container::checkCollisionsRegion(int startX, int endX, int startY, int endY)
    {
        for (int j=startY; j<endY; j++)
        {
            for (int i=startX; i<endX; i++)
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

            int minX = cellX - 1;
            int maxX = cellX + 1;
            if (minX < 0) minX = 0;
            if (maxX >= cellGrid_.getWidth()) maxX = cellGrid_.getWidth() - 1;

            int minY = cellY - 1;
            int maxY = cellY + 1;
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
            Vec2 overlapVector = pOtherObject->getPosition() - pObject->getPosition();   // Relative positions of the centers

            float distanceSquared = overlapVector.distanceSquared(Vec2::Zero());
            float totalRadiusSquared = totalRadius * totalRadius;

            if (totalRadiusSquared > distanceSquared)
            {
                float distance = sqrt(distanceSquared);      // Scalar distance between centers
                float overlap = totalRadius - distance;      // Amount of overlap (positive = overlap, negative = space between)

                Vec2 overlapUnit = (distance == 0) ? Vec2(0,-1) : overlapVector / distance;                                                    // Unit vector of their relative positions
                float ratio1 = pOtherObject->getRadius() / (pObject->getRadius() + pOtherObject->getRadius());  // Mass ratio for object 1
                float ratio2 = pObject->getRadius()      / (pObject->getRadius() + pOtherObject->getRadius());  // Mass ratio for object 2
                Vec2 reaction = overlapUnit * overlap * 0.5f * bounciness_;                                     // Reaction force, directional

                pObject->setPosition(pObject->getPosition() - (reaction * ratio1));
                pOtherObject->setPosition(pOtherObject->getPosition() + (reaction * ratio2));
            }

            numChecks++;
        }
    }

    void  Container::checkCircleConstraint(Object* pObject)
    {
        float constraintRadius = constraint_.width/2.0f;
        Vec2 positionRelative = pObject->getPosition() - constraint_.position;    // Position of object relative to constraint center
        float distanceSquared = (positionRelative.x * positionRelative.x) + (positionRelative.y * positionRelative.y);   // Distance squared from constraint center
        float radiusCombo = constraintRadius - pObject->getRadius();
        float radiusComboSquared = radiusCombo * radiusCombo;

        if (distanceSquared > radiusComboSquared)
        {
            // Move out by exactly the overshoot
            float distance = sqrt(distanceSquared);
            Vec2 newPosition = Vec2::Zero();
            if (distance > 0)
            {
                newPosition = (positionRelative / distance) * (constraintRadius - pObject->getRadius());
            }
            
            newPosition = newPosition + constraint_.position;
            pObject->setPosition(newPosition);
        }
    }

    void  Container::checkRectConstraint(Object* pObject)
    {
        float newX = pObject->getPosition().x;
        float newY = pObject->getPosition().y;

        if (newX < constraint_.position.x + pObject->getRadius())
        {
            newX = constraint_.position.x + pObject->getRadius();
        }

        if (newX > constraint_.position.x + constraint_.width - pObject->getRadius())
        {
            newX = constraint_.position.x + constraint_.width - pObject->getRadius();
        }

        if (newY < constraint_.position.y + pObject->getRadius())
        {
            newY = constraint_.position.y + pObject->getRadius();
        }

        if (newY > constraint_.position.y + constraint_.height - pObject->getRadius())
        {
            newY = constraint_.position.y + constraint_.height - pObject->getRadius();
        }

        pObject->setPosition(Vec2(newX, newY));
    }

    void Container::checkConstraints()
    {
        if (constraint_.isCircle)
        {
            for (Object& o: objects_)
            {
                checkCircleConstraint(&o);
            }
        }
        else
        {
            for (Object& o: objects_)
            {
                checkRectConstraint(&o);
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