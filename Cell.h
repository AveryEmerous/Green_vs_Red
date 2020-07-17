//
// Created by avery on 16.07.20 г..
//

#ifndef GREEN_VS_RED_CELL_H
#define GREEN_VS_RED_CELL_H

#include <vector>
#include <memory>

enum CELL_CHARS {
    NONE = '\0',
    RED = '0',
    GREEN = '1'
};

struct CellBoard;

class Cell {
public:
    Cell(size_t x, size_t y, char shape);

    virtual char nextGen(CellBoard & in) = 0;

    int getX() const;
    int getY() const;
    char getShape() const;

private:
    size_t x, y;
    char shape;
};

class Red_Cell : public Cell {
public:
    Red_Cell(size_t x, size_t y);

    char nextGen(CellBoard & in) override;

};

class Green_Cell : public Cell {
public:
    Green_Cell(size_t x, size_t y);

    char nextGen(CellBoard & in) override;
};

struct CellBox {
    CellBox(size_t upperY, size_t upperX, size_t lowerY, size_t lowerX);
    std::pair<size_t, size_t> upper;
    std::pair<size_t, size_t> lower;
    
};

struct CellBoard {
    CellBoard(size_t sizeX, size_t sizeY);

    void print();
    void clear();
    void update();

    template<class T>
    void fillEmpty() {
        static_assert(std::is_base_of<Cell, T>::value, "Type must be derived from Cell");

        for (size_t y = 0; y < height; y++) {
            for (size_t x = 0; x < width; x++) {
                if (!board[y][x]) {
                    board[y][x] = std::make_unique<T>(y, x);
                }
            }
        }
    }

    void addCell(std::unique_ptr<Cell> in);

    std::pair<int,int> getSize();

    CellBox cellArea(const Cell & in) const;

    char getCellShape(size_t y, size_t x) const;

private:
    std::vector<std::vector<std::unique_ptr<Cell>>> board;
    size_t height;
    size_t width;
};

#endif //GREEN_VS_RED_CELL_H
