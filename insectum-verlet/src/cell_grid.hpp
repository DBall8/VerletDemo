#pragma once
#include <vector>
#include "object.hpp"
#include <iostream>

namespace verlet
{
    class Cell
    {
        public:
            const static int MAX_OBJECT_PER_CELL = 100;
            Cell(){}

            void addObject(Object* po)
            {
                objects_.push_back(po);
            }

            Object* removeObject()
            {
                if (objects_.size() <= 0) return nullptr;

                Object* po = objects_.back();
                objects_.pop_back();
                return po;
            }
            std::vector<Object*> getObjects(){ return objects_; }
            int getNumObjects(){ return objects_.size(); }
            void clear(){ objects_.clear(); }

        private:
            std::vector<Object*> objects_;
    };

    class CellGrid
    {
        public:
            CellGrid(){}
            int getWidth(){ return width_; }
            int getHeight(){ return height_; }
            int getCellX(float worldX);
            int getCellY(float worldY);
            void addObject(Object* pObject);

            Cell* getCell(const unsigned int x, const unsigned int y);

            void clear();
            void resize(float cellSize, float fullWidth, float fullHeight);

        private:
            float cellSize_ = 0;
            int width_ = 0;
            int height_ = 0;
            Cell** grid_ = nullptr;
    };
}