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
                if (numObjects_ >= MAX_OBJECT_PER_CELL) return;
                objects_[numObjects_] = po;
                numObjects_++;
            }

            Object* removeObject()
            {
                if (numObjects_ <= 0) return nullptr;

                numObjects_--;
                Object* po = objects_[numObjects_];
                return po;
            }
            Object** getObjects(){ return objects_; }
            int getNumObjects(){ return numObjects_; }
            void clear(){ numObjects_ = 0; }

        private:
            Object* objects_[MAX_OBJECT_PER_CELL] = {nullptr};
            int numObjects_ = 0;
    };

    class CellGrid
    {
        public:
            CellGrid(){}
            int getWidth(){ return width_; }
            int getHeight(){ return height_; }
            int getCellSize(){ return cellSize_; }
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
            Cell* grid_ = nullptr;

            const int getCellIndex(const int x, const int y);
    };
}