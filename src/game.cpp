#include "game.hpp"

#include <algorithm>
#include <array>
#include <print>

#include "types.hpp"

void Game::printBoard() const {
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
bool Game::isValid(unsigned int col, unsigned int row, int value) const {
    if (value == 0) return true;
    int startCol = (col / BLOCK_SIZE) * 3;
    int startRow = (row / BLOCK_SIZE) * 3;
    // check vertically
    for (size_t y = 0; y < GRID_SIZE; y++) {
        if (value == m_board[col][y] && y != row) return false;
    }
    // check horizontally
    for (size_t x = 0; x < GRID_SIZE; x++) {
        if (value == m_board[x][row] && x != col) return false;
    }
    // check block
    for (size_t x = startCol; x < startCol + 3; x++) {
        for (size_t y = startRow; y < startRow + 3; y++) {
            if (value == m_board[x][y] && x != col && y != row) return false;
        }
    }
    return true;
}

// returns true on success
bool Game::setValue(unsigned int col, unsigned int row, int value) {
    if (!isValid(col, row, value)) {
        return false;
    }
    m_board[col][row] = value;
    return true;
}

void Game::generateSudoku() {
    // first fill a board then remove values
    fillBoard(0, 0);
}

// makes a copy of board and takes in position of current cell
bool Game::fillBoard(unsigned int col, unsigned int row) {
    if (col == GRID_SIZE) {
        col = 0;
        row++;
        if (row == GRID_SIZE) return true;
    }

    // If cell already has a number (shouldn't happen for generation, but safe)
    if (m_board[col][row] != 0) {
        return fillBoard(col + 1, row);
    }

    std::array<int, 9> possibleValues = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::shuffle(possibleValues.begin(), possibleValues.end(), m_randEngine);

    for (unsigned int value : possibleValues) {
        if (isValid(col, row, value)) {
            m_board[col][row] = value;
            if (fillBoard(col + 1, row)) return true;
        }
    }
    m_board[col][row] = 0;
    return false;
}

int Game::genValue() { return m_uniformDist(m_randEngine); }

void Game::clearBoard() {
    for (auto& col : m_board) col.fill(0);
}

int Game::readValue(unsigned int col, unsigned row) const { return m_board[col][row]; }
