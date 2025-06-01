#include "cell_grid.hpp"

namespace verlet
{
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
        grid_[cellY][cellX].addObject(pObject);
    }

    Cell* CellGrid::getCell(const unsigned int x, const unsigned int y)
    {
        if (x >= width_ && y >= height_)
        {
            return nullptr;
        }

        return &(grid_[y][x]);
    }

    void CellGrid::clear()
    {
        for (int j=0; j<height_; j++)
        {
            for (int i=0; i<width_; i++)
            {
                grid_[j][i].clear();
            }
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

        for (int j=0; j<height_; j++)
        {
            delete[] grid_[j];
        }

        if (height_ > 0 && width_ > 0 && grid_ != nullptr)
        {
            delete[] grid_;
            grid_ = nullptr;
        }

        cellSize_ = 0;
        width_ = 0;
        height_ = 0;

        grid_ = new Cell*[newHeight];
        if (grid_ == nullptr)
        {
            std::cout <<"FAILED TO ALLOC\n";
            return;
        }

        for (int j=0; j<newHeight; j++)
        {
            grid_[j] = new Cell[newWidth];
            if (grid_[j] == nullptr)
            {
                std::cout <<"FAILED TO ALLOC\n";
                return;
            }
        }

        cellSize_ = cellSize;
        width_ = newWidth;
        height_ = newHeight;
    }
}