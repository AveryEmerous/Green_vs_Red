#include "Cell.h"
#include <stdexcept>
#include <iostream>

CellBoard::CellBoard(size_t height, size_t width)
: height(height), width(width), nGen(0)
{
    if (0 == height || 0 == width)
        throw std::invalid_argument("Unable to construct board with a size of zero.");

    for (unsigned i = 0; i < width; i++) { // Allocating columns while abiding by ownership.
        std::vector<std::unique_ptr<Cell>> col;
        for (unsigned j = 0; j < height; j++) {
            col.push_back(std::unique_ptr<Cell>(nullptr));
        }
        board.push_back(std::move(col));
    }
}

void CellBoard::print() {
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            if (nullptr == board[y][x]) { // Representing empty cells.
                std::cout << ".\t";
                continue;
            }
            std::cout << board[y][x]->getShape() << '\t';
        }
        std::cout << '\n';
    }
}

void CellBoard::clear() {
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            board[y][x].reset();
        }
    }
}

void CellBoard::update() {
    nGen++;
    char shape = '\0';
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            if (board[y][x]) { // Check for a nullptr.
                if ((shape = board[y][x]->nextGen(*this)) != board[y][x]->getShape()) {
                    switch (shape) {
                        case RED:
                            changeCellTo<Red_Cell>(y,x);
                            break;

                        case GREEN:
                            changeCellTo<Green_Cell>(y,x);
                            break;

                        default:
                            break;
                    }
                }
            }
        }
    }
}

size_t CellBoard::getGens() const {
    return nGen;
}

void CellBoard::addCell(std::unique_ptr<Cell> in) {
    if (!in) return;
    if ((in->getY() < height || in->getX() < width) && !board[in->getY()][in->getX()])
        board[in->getY()][in->getX()] = std::move(in);
}

std::pair<int, int> CellBoard::getSize() const {
    return {height, width};
}

CellBox CellBoard::getCellArea(const Cell & in) const {
    if (in.getY() >= height || in.getX() >= width) {
        throw std::out_of_range("Out of bounds.");
    } else
        if ((in.getY() > 0 && in.getY() < height - 1) && (in.getX() > 0 && in.getX() < width - 1)) {
            return CellBox(in.getY() - 1, in.getX() - 1, in.getY() + 1, in.getX() + 1);
    } else
        if (in.getY() == height - 1 && in.getX() == width - 1) {
            return CellBox(in.getY() - 1, in.getX() - 1, in.getY(), in.getX());
    } else
        if (in.getX() == width - 1) {
            return CellBox(in.getY() - 1, in.getX() - 1, in.getY() + 1, in.getX());
    } else
        if (in.getY() == height - 1) {
            return CellBox(in.getY() - 1, in.getX() - 1, in.getY(), in.getX() + 1);
    } else
        if (in.getX() == 0) {
            return CellBox(in.getY() - 1, in.getX(), in.getY() + 1, in.getX() + 1);
    } else
        if (in.getY() == 0) {
            return CellBox(in.getY(), in.getX() - 1, in.getY() + 1, in.getX() + 1);
    } else
        return CellBox(in.getY(), in.getX(), in.getY() + 1, in.getX() + 1);

}

char CellBoard::getCellShape(size_t y, size_t x) const {
    return (board[y][x]) ? board[y][x]->getShape() : NONE;
}

CellBox::CellBox(size_t upperY, size_t upperX, size_t lowerY, size_t lowerX)
        : upper(upperY, upperX), lower(lowerY, lowerX)
{
}
