#include "cell_grid.hpp"

namespace verlet
{
    const int CellGrid::getCellIndex(const int x, const int y)
    {
        //return x * height_ + y;
        return y * width_ + x;
    }

    int CellGrid::getCellX(float worldX)
    {
        if (worldX == 0) return 0;
        return floor(worldX / cellSize_);
    }

    int CellGrid::getCellY(float worldY)
    {
        if (worldY == 0) return 0;
        return floor(worldY / cellSize_);
    }

    void CellGrid::addObject(Object* pObject)
    {
        int cellX = getCellX(pObject->getPosition().x);
        int cellY = getCellY(pObject->getPosition().y);

        if (cellX < 0) cellX = 0;
        if (cellY < 0) cellY = 0;
        if (cellX >= width_) cellX = width_ - 1;
        if (cellY >= height_) cellY = height_ - 1;
        grid_[getCellIndex(cellX, cellY)].addObject(pObject);
    }

    Cell* CellGrid::getCell(const unsigned int x, const unsigned int y)
    {
        if (x >= width_ && y >= height_)
        {
            return nullptr;
        }

        return &(grid_[getCellIndex(x, y)]);
    }

    void CellGrid::clear()
    {
        for (int i=0; i<width_*height_; i++)
        {
            grid_[i].clear();
        }
    }

    void CellGrid::resize(float cellSize, float fullWidth, float fullHeight)
    {

        int newWidth = ceil(fullWidth / cellSize);
        int newHeight = ceil(fullHeight / cellSize);

        if (newWidth < 0 || newHeight < 0) return;

        if ((width_ == newWidth) &&
            (height_ == newHeight) &&
            (cellSize_ == cellSize))
        {
            // No change
            return;
        }

        if (height_ > 0 && width_ > 0 && grid_ != nullptr)
        {
            delete[] grid_;
            grid_ = nullptr;
        }

        cellSize_ = 0;
        width_ = 0;
        height_ = 0;

        grid_ = new Cell[newHeight*newWidth];
        if (grid_ == nullptr)
        {
            std::cout <<"FAILED TO ALLOC\n";
            return;
        }
        
        cellSize_ = cellSize;
        width_ = newWidth;
        height_ = newHeight;
    }
}