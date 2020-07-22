#include "Cell.h"
#include <stdexcept>
#include <iostream>

Cell::Cell(size_t y, size_t x, char shape)
: y(y), x(x), shape(shape), futureCell(nullptr)
{
}

std::unique_ptr<Cell> Cell::moveFutureCell() {
    return std::move(futureCell);
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

void Red_Cell::nextGen(CellBoard &board) {
    CellBox currBox = board.getCellArea(*this);
    size_t greenCnt = 0;
    for (unsigned y = currBox.upper.first; y <= currBox.lower.first; y++) {
        for (unsigned x = currBox.upper.second; x <= currBox.lower.second; x++) {
            if (board.getCellShapeAt(y, x) == GREEN) {
                greenCnt++;
            }
        }
    }

    switch (greenCnt) {
        case 3: case 6:
            futureCell = std::make_unique<Green_Cell>(getY(),getX());
        default:
            return;
    }
}

Green_Cell::Green_Cell(size_t y, size_t x)
: Cell(y, x, GREEN)
{
}

void Green_Cell::nextGen(CellBoard &board) {
    CellBox currBox = board.getCellArea(*this);
    size_t greenCnt = 0;
    for (unsigned y = currBox.upper.first; y <= currBox.lower.first; y++) {
        for (unsigned x = currBox.upper.second; x <= currBox.lower.second; x++) {
            if (board.getCellShapeAt(y, x) == GREEN) {
                greenCnt++;
            }
        }
    }

    greenCnt--; // A green cell will always count at least itself once.

    switch (greenCnt) {
        case 2: case 3: case 6:
            return;
        default:
            futureCell = std::make_unique<Red_Cell>(getY(), getX());
    }
}