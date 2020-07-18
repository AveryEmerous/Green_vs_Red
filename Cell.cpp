#include "Cell.h"
#include <stdexcept>
#include <iostream>

Cell::Cell(size_t y, size_t x, char shape)
: y(y), x(x), shape(shape)
{
}

int Cell::getX() const {
    return x;
}

int Cell::getY() const {
    return y;
}

char Cell::getShape() const {
    return shape;
}

Red_Cell::Red_Cell(size_t y, size_t x)
: Cell(y, x, RED)
{
}

std::unique_ptr<Cell> Red_Cell::nextGen(CellBoard &in) {
    CellBox currBox = in.getCellArea(*this);
    unsigned greenCnt = 0;
    for (unsigned y = currBox.upper.first; y < currBox.lower.first; y++) {
        for (unsigned x = currBox.upper.second; x < currBox.lower.second; x++) {
            if (in.getCellShape(y, x) == GREEN) {
                greenCnt++;
            }
        }
    }

    switch (greenCnt) {
        case 3: case 6:
            return nullptr;
        default:
            return std::make_unique<Green_Cell>(getY(), getX());
    }

}

Green_Cell::Green_Cell(size_t y, size_t x)
: Cell(y, x, GREEN)
{
}

std::unique_ptr<Cell> Green_Cell::nextGen(CellBoard &in) {
    CellBox currBox = in.getCellArea(*this);
    unsigned redCnt = 0;
    for (unsigned y = currBox.upper.first; y < currBox.lower.first; y++) {
        for (unsigned x = currBox.upper.second; x < currBox.lower.second; x++) {
            if (in.getCellShape(y, x) == GREEN) {
                redCnt++;
            }
        }
    }

    switch (redCnt) {
        case 2: case 3: case 6:
            return nullptr;

        default:
            return std::make_unique<Red_Cell>(getY(), getX());
    }
}