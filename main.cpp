// Made by Daniel Ivanov with g++ version 9.3.0
// github.com/AveryEmerous

#include <iostream>
#include <memory>
#include <algorithm>
#include "Cell.h"

// Extracts all of the numbers in the stream and allocates the
// given amount of space for different numbers. Different numbers
// need to be separated in the stream by some kind of non-digit character.
std::vector<size_t> getnums(size_t n_nums) {
    std::vector<size_t> out;
    std::string buffer, tmp;
    getline(std::cin, buffer);

    for (size_t i = 0, j = 0; i <= buffer.length(); i++) { // j - copy from, i - copy to
        if (!isdigit(buffer[i])) {
            tmp = buffer.substr(j, i);
            out.push_back(atoi(tmp.c_str()));
            if (out.size() >= n_nums)
                break;

            while (!isdigit(buffer[i])) { // Skips all of the non-digit characters.
                i++;
            }
            j = i;
        }
    }
    return out;
}

int main() {

    while (true) {
        try {
            size_t y, x, cntGens, greenCnt = 0;
            std::vector<size_t> tmp;

            std::cout << "Enter grid size (x,y):\n";

            tmp = getnums(2);

            CellBoard board(tmp[0], tmp[1]);

            std::cin >> board;

            tmp.clear();
            tmp = getnums(3);
            x = tmp[0];
            y = tmp[1];
            cntGens = tmp[2];

            while (board.getGens() < cntGens) {
                board.update();
                if (board.getCellShapeAt(y, x) == GREEN)
                    greenCnt++;
            }

            std::cout << "Green occurrences at [" << x << ", " << y << "] is: " << greenCnt << '\n';

            return 0;

        } catch (std::out_of_range & e) {
            std::cerr << "An error occurred: " << e.what() << '\n'
                << "A cell is out of the bounds of the board.\n";
        } catch (std::exception &e) {
            std::cerr << "An error occurred: " << e.what() << '\n';
        }
    }

    return 0;
}
