// Made by Daniel Ivanov with g++ version 9.3.0
// github.com/AveryEmerous

#ifndef GREEN_VS_RED_CELL_H
#define GREEN_VS_RED_CELL_H

#define MAX_SIZE_Y 1000
#define MAX_SIZE_X 1000

#include <vector>
#include <memory>
#include <iostream>

// Easier connection with the colors than arbitrary characters.
enum CELL_CHARS {
    NONE    = '.',
    RED     = '0',
    GREEN   = '1'
};

// Defined prototype for the compiler to see that it will be defined later
// and able to use it in code before it's definition.
struct CellBoard;

// Base class of all cells. Used to define newer classes with inherited logic
// behind them and ability to store them together in arrays.
class Cell {
public:
    // Initializes the private variables.
    Cell(size_t y, size_t x, char shape);

    // Virtual destructor for in the case that a future derived type needs a destructor.
    virtual ~Cell() = default;

    // Contains logic associated with the type of cell used.
    // Extracts the cells around it through the given board
    // and returns to which shape/color it should turn into.
    virtual void nextGen(CellBoard & board) = 0;

    // Moves the contents of the private future cell.
    // Nullptr if it the current cell doesn't change it's type.
    std::unique_ptr<Cell> getFutureCell();

    // Getters:
    int getX() const;
    int getY() const;
    char getShape() const;

protected:
    std::unique_ptr<Cell> futureCell;

private:
    const size_t  x, y;
    const char    shape;

};

// Derived class from Cell.
// Exhibits the rules:
//  1)  Change to green cell whenever there are 3 or 6 green cells
//      surrounding the current cell.
//  2)  Will not change cell shape/color whenever there are any other
//      amount of cells surrounding the current cell.
class Red_Cell : public Cell {
public:
    Red_Cell(size_t y, size_t x);

    void nextGen(CellBoard & board) override;

};

// Derived class from Cell.
// Exhibits the rules:
//  1)  Stay green whenever there are 2, 3 or 6 green cells surrounding
//      the current cell.
//  2)  Will change cell "color" whenever there are any other amount
//      of cells surrounding the current cell.
class Green_Cell : public Cell {
public:
    Green_Cell(size_t y, size_t x);

    void nextGen(CellBoard & board) override;

};

// Contains the boundaries of where a cell should check it's surroundings.
struct CellBox {
    CellBox(size_t upperY, size_t upperX, size_t lowerY, size_t lowerX);
    std::pair<size_t, size_t> upper;
    std::pair<size_t, size_t> lower;
    
};

// Contains all of the cells and the methods by which they can communicate.
// MAX_SIZE: 999x999
class CellBoard {
public:
    // Initializes the 2D vector to the appropriate size.
    CellBoard(size_t width, size_t height);

    // Prints the whole board to the console with tabulations.
    void print();

    // Clears out all of the cells and replaces them with empty ones (nullptr).
    void clear();

    // Initializes a single generation.
    void update();

    // Returns the number of generations the board has gone through.
    size_t getGens() const;

    // Returns the shape/color of the cell in coordinates.
    char getCellShapeAt(size_t y, size_t x) const;

    // Returns the boundaries of the searched area for a cell.
    CellBox getCellArea(const Cell & in) const;

    // Returns the size of the board with format (HEIGHT, WIDTH).
    std::pair<int,int> getSize() const;

    // Adds a cell to the stored coordinates if the space is empty (nullptr).
    void addCell(std::unique_ptr<Cell> in);

    // Overwrites the cell that is in the given coordinates.
    void changeCell(std::unique_ptr<Cell> in);

    // Fills all of the remaining unused spots with a Cell derivative.
    template<class T>
    void fillEmptyWith() {
        static_assert(std::is_base_of<Cell, T>::value, "Type must be derived from Cell");
        for (size_t y = 0; y < height; y++) {
            for (size_t x = 0; x < width; x++) {
                if (!board[y][x]) {
                    board[y][x] = std::make_unique<T>(y, x);
                }
            }
        }
    }

    // Fills the board cells with the inputted characters and their relative class.
    // Only accept default cell types. Change function declaration to accept more types.
    void fillFromConsole();

    // iostream operator handlers:
    std::ostream & ext (std::ostream & os) const;
    std::istream & ins (std::istream & is);

private:
    std::vector<std::vector<std::unique_ptr<Cell>>> board;
    const size_t height;
    const size_t width;
    size_t nGen;

};

// Redefinitions:
std::ostream & operator<< (std::ostream & os, const CellBoard & obj);
std::istream & operator>> (std::istream & is, CellBoard & obj);

#endif //GREEN_VS_RED_CELL_H
