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

    board.addCell(std::make_unique<Green_Cell>(1,0)); //Example 1 (3x3, 10)
    board.addCell(std::make_unique<Green_Cell>(1,1));
    board.addCell(std::make_unique<Green_Cell>(1,2));

    board.fillEmptyWith<Red_Cell>();

/*    board.addCell(std::make_unique<Red_Cell>(0,1)); // example 2 (4x4, 15)
    board.addCell(std::make_unique<Red_Cell>(0,2));
    board.addCell(std::make_unique<Red_Cell>(2,0));
    board.addCell(std::make_unique<Red_Cell>(2,2));
    board.addCell(std::make_unique<Red_Cell>(2,3));
    board.addCell(std::make_unique<Red_Cell>(3,1));
    board.addCell(std::make_unique<Red_Cell>(3,3));

    board.fillEmptyWith<Green_Cell>();*/

    unsigned cnt = 0;

/*    board.fillFromConsole();*/

    for (unsigned i = 0; i < 10; i++) {
/*        if (board.getCellShapeAt(2,2) == GREEN) {
            *//*std::cout << '\n' << *//*cnt++*//* << '\n'*//*;
        }*/

        std::cout << '\n';
        board.print();
        std::cout << '\n' << "Generation: " << i+1 << '\n';
        board.update();

        if (board.getCellShapeAt(0, 1) == GREEN) {
            /*std::cout << '\n' << */cnt++/* << '\n'*/;
        }

/*        {
            std::cout << '\n' << "Press Enter to continue to next generation...";
            while (std::cin.get() != '\n');
        }*/

    }

    std::cout << "Times green: " << cnt << '\n';

    std::string name;

    return 0;
}
