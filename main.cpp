#include <iostream>
#include <memory>
#include "Cell.h"

int main() {

/*    int w = 5, h = 5;

    std::vector<std::vector<std::unique_ptr<Cell>>> B;

    for (unsigned i = 0; i < w; i++) {
        std::vector<std::unique_ptr<Cell>> col;
        for (unsigned j = 0; j < h; j++) {
            col.push_back(std::unique_ptr<Cell>(nullptr));
        }
        B.push_back(std::move(col));
    }

    B[2][2] = std::make_unique<Red_Cell>(2,2);*/

    CellBoard board (3,3);

    //board.addCell(std::make_unique<Red_Cell>(0,0));
    board.addCell(std::make_unique<Green_Cell>(1,0));
    board.addCell(std::make_unique<Green_Cell>(1,1));
    board.addCell(std::make_unique<Green_Cell>(1,2));

    board.fillEmpty<Red_Cell>();

    unsigned cnt = 0;

    for (unsigned i = 0; i < 10; i++) {
        if (board.getCellShape(0,1) == GREEN) {
            std::cout << '\n' << cnt++ << '\n';
        }

        board.print();
        std::cout << '\n';
        board.update();
    }

    std::cout << cnt << '\n';

    return 0;
}
