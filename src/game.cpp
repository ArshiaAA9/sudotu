#include "game.hpp"

#include <iostream>
#include <print>

void Game::printBoard() {
    std::print("+---------+---------+---------+\n");
    for (size_t y = 0; y < GRID_SIZE; y++) {
        std::print("|");
        for (size_t x = 0; x < GRID_SIZE; x++) {
            std::print(" {} ", m_board[x][y]);
            if ((x + 1) % BLOCK_SIZE == 0) {
                std::print("|");
            }
        }
        std::print("\n");
        if ((y + 1) % BLOCK_SIZE == 0) {
            std::print("+---------+---------+---------+\n");
        }
    }
}

// col is x value row is y
bool Game::isValid(unsigned int col, unsigned int row, int value) {
    if (value == 0) return true;
    int startCol = (col / BLOCK_SIZE) * 3;
    int startRow = (row / BLOCK_SIZE) * 3;
    // check vertically
    for (size_t y = 0; y < GRID_SIZE; y++) {
        if (value == m_board[col][y]) return false;
    }
    // check horizontally
    for (size_t x = 0; x < GRID_SIZE; x++) {
        if (value == m_board[x][row]) return false;
    }
    // check block
    for (size_t x = startCol; x < startCol + 3; x++) {
        for (size_t y = startRow; y < startRow + 3; y++) {
            if (value == m_board[x][y]) return false;
        }
    }
    return true;
}

// returns true on success
bool Game::insertValue(unsigned int col, unsigned int row, int value) {
    if (!isValid(col, row, value) || m_board[col][row] != 0) {
        return false;
    }
    m_board[col][row] = value;
    return true;
}

bool Game::changeValue(unsigned int col, unsigned int row, int value) {
    if (!isValid(col, row, value)) {
        return false;
    }
    m_board[col][row] = value;
    return true;
}
