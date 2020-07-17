#include "Cell.h"
#include <stdexcept>
#include <iostream>

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

Cell::~Cell()
{
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

CellBoard::CellBoard(size_t sizeX, size_t sizeY)
: width(sizeX), height(sizeY), nGen(0)
{
    for (unsigned i = 0; i < width; i++) {
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
            if (nullptr == board[y][x]) {
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
    for (size_t h = 0; h < height; h++) {
        for (size_t w = 0; w < width; w++) {
            if (board[h][w]) {
                if ((shape = board[h][w]->nextGen(*this)) != board[h][w]->getShape()) {
                    switch (shape) {
                        case RED:
                            board[h][w].reset();
                            board[h][w] = std::make_unique<Red_Cell>(h,w);
                            break;

                        case GREEN:
                            board[h][w].reset();
                            board[h][w] = std::make_unique<Green_Cell>(h,w);
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
    if (in.getX() < width && in.getY() < height) { // Base case with the cell being in the board and not on and edge.
        return {height, width,
                height, width};
    } else
        if (in.getX() == width && in.getY() == height) { // Cell being on the bottom right corner.
            return {height - 1, width - 1,
                    height, width};
    } else
        if (in.getX() == width) { // Cell being on the right edge.
            return {height - 1, width - 1,
                    height, width + 1};
    } else
        if (in.getY() == height) { // Cell being on the bottom edge.
            return {height - 1, width - 1,
                    height + 1, width};
    } else
        if (in.getX() == 0 && in.getY() == 0) { // Cell being on the top left corner.
            return {height, width,
                    height + 1, width + 1};
    } else
        if (in.getX() == 0) { // Cell being on the left edge.
            return {height - 1, width,
                    height + 1, width + 1};
    } else
        if (in.getY() == 0) { // Cell being on the top edge.
            return {height, width - 1,
                    height + 1, width + 1};
    } else // Cell being outside of the board area.
        throw std::runtime_error("Invalid cell coordinates.");
}

char CellBoard::getCellShape(size_t y, size_t x) const {
    return (board[y][x]) ? board[y][x]->getShape() : NONE;
}

CellBox::CellBox(size_t upperY, size_t upperX, size_t lowerY, size_t lowerX)
: upper(upperY, upperX), lower(lowerY, lowerX)
{
}
