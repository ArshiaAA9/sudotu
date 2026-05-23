#pragma once
#include "types.hpp"

class Game {
public:
    Game() {};
    void printBoard();
    bool insertValue(unsigned int col, unsigned int row, int value);
    bool changeValue(unsigned int col, unsigned int row, int value);

private:
    bool isValid(unsigned int col, unsigned int row, int value);

    BoardType m_board{};
};
