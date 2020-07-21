#include "Cell.h"
#include <iostream>
#include <stdexcept>
#include <string>

CellBoard::CellBoard(size_t height, size_t width)
: height(height % MAX_SIZE_Y), width(width % MAX_SIZE_X), nGen(0)
{
    if (0 == this->height || 0 == this->width)
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
            if (!board[y][x]) { // Representing empty cells.
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
    std::unique_ptr<Cell> newCell = nullptr;
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            if (board[y][x] && (newCell = board[y][x]->nextGen(*this))) {
                changeCell(std::move(newCell));
            }
        }
    }
}

size_t CellBoard::getGens() const {
    return nGen;
}

void CellBoard::addCell(std::unique_ptr<Cell> in) {
    if (!in) return;
    if ((in->getY() < height && in->getX() < width) && !board[in->getY()][in->getX()])
        board[in->getY()][in->getX()] = std::move(in);
}

void CellBoard::changeCell(std::unique_ptr<Cell> in) {
    board[in->getY()][in->getX()].reset();
    addCell(std::move(in));
}

std::pair<int, int> CellBoard::getSize() const {
    return {height, width};
}

CellBox CellBoard::getCellArea(const Cell & in) const {
    if (in.getY() >= height || in.getX() >= width) {
        throw std::out_of_range("Out of bounds.");
    } else if ((in.getY() > 0 && in.getY() < height - 1) && (in.getX() > 0 && in.getX() < width - 1)) {
        // Within borders and on none of the edges/corners.
        return CellBox(in.getY() - 1, in.getX() - 1, in.getY() + 1, in.getX() + 1);
    } else if (in.getY() == height - 1 && in.getX() == width - 1) { // Bottom-right corner.
        return CellBox(in.getY() - 1, in.getX() - 1, in.getY(), in.getX());
    } else if (in.getY() == 0 && in.getX() == width - 1) { // Top-right corner.
        return CellBox(in.getY(), in.getX() - 1, in.getY() + 1, in.getX());
    } else if (in.getY() == height - 1 && in.getX() == 0) { // Bottom-left corner.
        return CellBox(in.getY() - 1, in.getX(), in.getY(), in.getX() + 1);
    } else if (in.getY() == 0 && in.getX() == 0) { // Top-left corner.
        return CellBox(in.getY(), in.getX(), in.getY() + 1, in.getX() + 1);
    } else if (in.getX() == width - 1) { // Right border.
        return CellBox(in.getY() - 1, in.getX() - 1, in.getY() + 1, in.getX());
    } else if (in.getY() == height - 1) { // Bottom border.
        return CellBox(in.getY() - 1, in.getX() - 1, in.getY(), in.getX() + 1);
    } else if (in.getX() == 0) { // Left border.
        return CellBox(in.getY() - 1, in.getX(), in.getY() + 1, in.getX() + 1);
    } else if (in.getY() == 0) { // Top border.
        return CellBox(in.getY(), in.getX() - 1, in.getY() + 1, in.getX() + 1);
    } else
        throw std::out_of_range("Unknown bounds.");
}

char CellBoard::getCellShape(size_t y, size_t x) const {
    if ((y < height && x < width) && board[y][x])
        return board[y][x]->getShape();
    else return NONE;
}


CellBox::CellBox(size_t upperY, size_t upperX, size_t lowerY, size_t lowerX)
: upper(upperY, upperX), lower(lowerY, lowerX)
{
}

void CellBoard::fillFromConsole() {
    std::string buffer;
    for (size_t y = 0; y < height; y++) {
        size_t x = 0;
        std::getline(std::cin, buffer);
        for (char c: buffer) {
            switch (c) {
                case ' ': case '\t':
                    break;

                case '0':
                    addCell(std::make_unique<Red_Cell>(y,x++));
                    break;

                case '1':
                    addCell(std::make_unique<Green_Cell>(y,x++));
                    break;

                default:
                    throw std::invalid_argument("Unknown cell shape/color.");
            }
            if (x >= width) break;
        }
        if (x < width)
            throw std::underflow_error("Not enough cells inputted.");
    }
}