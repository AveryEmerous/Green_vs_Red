#include "Cell.h"
#include <iostream>
#include <stdexcept>
#include <string>

CellBoard::CellBoard(size_t width, size_t height)
: width(width % MAX_SIZE_Y), height(height % MAX_SIZE_X), nGen(0)
{
    if (0 == this->height || 0 == this->width)
        throw std::invalid_argument("Invalid size to construct board.");

    for (unsigned i = 0; i < height; i++) { // Allocating columns while abiding by ownership.
        std::vector<std::unique_ptr<Cell>> col;
        for (unsigned j = 0; j < width; j++) {
            col.push_back(std::unique_ptr<Cell>(nullptr));
        }
        board.push_back(std::move(col));
    }
}

void CellBoard::print() {
    std::cout << *this;
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
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            if (board[y][x]) {
                board[y][x]->nextGen(*this);
            }
        }
    }

    std::unique_ptr<Cell> newCell = nullptr;
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            if (board[y][x] && (newCell = std::move(board[y][x]->getFutureCell()))) {
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
    if (in.getX() > width || in.getX() < 0 || in.getY() >= height || in.getY() < 0)
        throw std::out_of_range("Out of bounds.");

    size_t lowx = (in.getX() - 1 > 0) ? in.getX() - 1 : 0;
    size_t lowy = (in.getY() - 1 > 0) ? in.getY() - 1 : 0;
    size_t highx = (in.getX() + 1 < width - 1) ? in.getX() + 1 : width - 1;
    size_t highy = (in.getY() + 1 < height - 1) ? in.getY() + 1 : height - 1;

    return {lowy,lowx,highy,highx};
}

char CellBoard::getCellShapeAt(size_t y, size_t x) const {
    if ((y < height && x < width) && board[y][x])
        return board[y][x]->getShape();
    else return NONE;
}


CellBox::CellBox(size_t upperY, size_t upperX, size_t lowerY, size_t lowerX)
: upper(upperY, upperX), lower(lowerY, lowerX)
{
}

void CellBoard::fillFromConsole() {
    std::cin >> *this;
}

std::ostream &CellBoard::ext(std::ostream &os) const {
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            if (board[y][x]) {
                os << board[y][x]->getShape() << '\t';
            }
        }
        os << '\n';
    }
    return os;
}

std::istream &CellBoard::ins(std::istream &is) {
    std::string buffer;
    for (size_t y = 0, x; y < height; y++) {
        x = 0;
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
        }
        if (x < width)
            throw std::underflow_error("Not enough cells inputted.");
    }

    return is;
}

std::ostream & operator<< (std::ostream & os, const CellBoard & obj) {return obj.ext(os);}
std::istream & operator>> (std::istream & is, CellBoard & obj) {return obj.ins(is);}
