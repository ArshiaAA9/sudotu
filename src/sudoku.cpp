#include "sudoku.hpp"

#include <sys/types.h>

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <numeric>
#include <print>
#include <random>

#include "types.hpp"

bool Sudoku::isInBound(unsigned int col, unsigned int row, unsigned int value) {
    return (col <= 8) && (row <= 8) && (value <= 9);
}

void Sudoku::printBoard() const {
    std::print("+---------+---------+---------+\n");
    for (size_t y = 0; y < GRID_SIZE; y++) {
        std::print("|");
        for (size_t x = 0; x < GRID_SIZE; x++) {
            if (m_board[x][y] == 0) std::print("   ");
            else
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
bool Sudoku::isValid(unsigned int col, unsigned int row, int value) const {
    assert((col < GRID_SIZE) && (row < GRID_SIZE) && (value <= 9));
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

bool Sudoku::setValue(unsigned int col, unsigned int row, unsigned int value) {
    assert((col < GRID_SIZE) && (row < GRID_SIZE) && (value <= 9));
    if (!isValid(col, row, value)) {
        return false;
    }
    m_board[col][row] = value;
    return true;
}

void Sudoku::generateSudoku(unsigned int difficulty) {
    // first generate a seed
    std::random_device rd;
    uint32_t randomSeed = rd();
    m_randEngine.seed(randomSeed);
    m_currentSeed = randomSeed;
    std::print("{}\n", randomSeed);

    // clear the board from before
    clearBoards();
    // generate a new one
    fillBoard(0, 0);
    m_filledBoard = m_board;
    // remove some values
    punchHoles(difficulty);
}

void Sudoku::generateSudoku(unsigned int difficulty, uint32_t seed) {
    m_randEngine.seed(seed);
    m_currentSeed = seed;
    std::print("{}\n", seed);

    // clear the board from before
    clearBoards();
    // generate a new one
    fillBoard(0, 0);
    m_filledBoard = m_board;
    // remove some values
    punchHoles(difficulty);
    m_emptyCells = difficulty;
}

// makes a copy of board and takes in position of current cell
bool Sudoku::fillBoard(unsigned int col, unsigned int row) {
    if (col == GRID_SIZE) {
        col = 0;
        row++;
        if (row == GRID_SIZE) return true;
    }

    // If cell already has a number (shouldn't happen for generation, but safe)
    if (m_board[col][row] != 0) {
        return fillBoard(col + 1, row);
    }

    std::array<unsigned int, GRID_SIZE> possibleValues = {1, 2, 3, 4, 5, 6, 7, 8, 9};
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

void Sudoku::punchHoles(unsigned int difficulty) {
    /*
     * so we dont need recursion we just need to iterate over the cells and punch
     * hole as many as we need to satisfy the difficulty if while we did that there
     * was case where removing that cell would stop giving us a unique solution we
     * just simply skip that cell and move to the next cell that we want to punch.
     * */
    std::array<unsigned int, 81> indices;
    std::iota(indices.begin(), indices.end(), 0);
    std::shuffle(indices.begin(), indices.end(), m_randEngine);

    unsigned int punchedHoles{};

    // possibe values to remove
    for (unsigned int index : indices) {
        if (punchedHoles == difficulty) return;
        unsigned int col = index % GRID_SIZE;
        // float point turnication gives us exact row
        unsigned int row = index / GRID_SIZE;
        unsigned int currentValue = m_board[col][row];
        m_board[col][row] = 0;
        // put back the value if not multiple solutions
        if (!hasOneSolution()) {
            m_board[col][row] = currentValue;
        } else {
            punchedHoles++;
        }
    }
}

bool Sudoku::hasOneSolution() {
    unsigned int uniqueSolutionCount{};
    solveForSolutions(0, 0, uniqueSolutionCount);
    return uniqueSolutionCount == 1;
}

void Sudoku::solveForSolutions(unsigned int col, unsigned int row, unsigned int& uniqueSolutionCount) {
    // if reached the end
    if (col == GRID_SIZE) {
        col = 0;
        row++;
        if (row == GRID_SIZE) {
            uniqueSolutionCount++;
            return;
        }
    }
    // skip non-empty cells
    if (m_board[col][row] != 0) {
        return solveForSolutions(col + 1, row, uniqueSolutionCount);
    }
    for (unsigned int value = 1; value <= GRID_SIZE; value++) {
        if (isValid(col, row, value)) {
            m_board[col][row] = value;
            solveForSolutions(col + 1, row, uniqueSolutionCount);
            m_board[col][row] = 0;
            if (uniqueSolutionCount == 2) return;
        }
    }
    return;
}

void Sudoku::clearBoards() {
    for (auto& col : m_board) col.fill(0);
    for (auto& col : m_filledBoard) col.fill(0);
}

int Sudoku::readValue(unsigned int col, unsigned row) const { return m_board[col][row]; }

MoveResult Sudoku::insertValue(unsigned int col, unsigned int row, unsigned int value) {
    assert((col < GRID_SIZE) && (row < GRID_SIZE) && (value <= 9));
    // check if the cell is empty
    // if yes insert the value
    // if the value doesnt equal the original filled board
    // update m_mistakes and return false
    if (m_board[col][row] != 0) {
        return MoveResult::NOT_EMPTY;
    }
    // if the value is the same in the filled board then its correct
    // otherwise its wrong and we add a mistake and increment the mistakes count
    if (value == m_filledBoard[col][row]) {
        m_board[col][row] = value;
        m_emptyCells--;
        if (m_emptyCells == 0) {
            // if there are no empty cells left the player has won
            return MoveResult::WIN;
        }
        return MoveResult::SUCCEED;
    } else {
        m_mistakesCount++;
        m_mistakes.insert(std::make_pair(m_mistakesCount, Mistake(col, row, value)));
        return MoveResult::MISTAKE;
    }
}
