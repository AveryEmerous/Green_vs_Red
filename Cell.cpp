#include "Cell.h"
#include <stdexcept>

Cell::Cell(size_t x, size_t y, char shape)
: x(x), y(y), shape(shape)
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

Red_Cell::Red_Cell(size_t x, size_t y)
: Cell(x, y, RED)
{
}

char Red_Cell::nextGen(CellBoard &in) {
    CellBox currBox = in.getCellArea(*this);
    unsigned greenCnt = 0;
    for (unsigned i = currBox.upper.first; i < currBox.lower.first; i++) {
        for (unsigned j = currBox.upper.second; j < currBox.lower.second; j++) {
            if (i == getX() && j == getY())
                continue;
            if (in.getCellShape(i,j) == GREEN) {
                greenCnt++;
            }
        }
    }

    return !(greenCnt % 3) ? GREEN : NONE;
}

Green_Cell::Green_Cell(size_t x, size_t y)
: Cell(x, y, GREEN)
{
}

char Green_Cell::nextGen(CellBoard &in) {
    CellBox currBox = in.getCellArea(*this);
    unsigned redCnt = 0;
    for (unsigned i = currBox.upper.first; i < currBox.lower.first; i++) {
        for (unsigned j = currBox.upper.second; j < currBox.lower.second; j++) {
            if (i == getX() && j == getY())
                continue;
            if (in.getCellShape(i,j) == GREEN) {
                redCnt++;
            }
        }
    }

    switch (redCnt) {
        case 2: case 3: case 6:
            return NONE;

        default:
            return RED;
    }
}