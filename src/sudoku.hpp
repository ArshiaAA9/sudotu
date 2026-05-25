#pragma once
#include <cstdint>
#include <random>

#include "types.hpp"

class Sudoku {
public:
    Sudoku() {};

    // tries to insert a value to a cell if sudoku rules allow it
    // if the cell is currently occupied it changes it
    bool setValue(unsigned int col, unsigned int row, int value);

    void generateSudoku(unsigned int difficulty);
    void generateSudoku(unsigned int difficulty, uint32_t seed);
    void clearBoard();
    void printBoard() const;

    bool isValid(unsigned int col, unsigned int row, int value) const;
    int readValue(unsigned int col, unsigned row) const;

    inline const BoardType& board() const { return m_board; }

private:
    bool fillBoard(unsigned int col, unsigned int row);
    void punchHoles(unsigned int difficulty);
    bool hasOneSolution();
    void solveForSolutions(unsigned int col, unsigned int row, unsigned int& uniqueSolutionCount);

    BoardType m_board{};
    std::mt19937 m_randEngine;
    uint32_t m_currentSeed;
    std::uniform_int_distribution<int> m_uniformDist{1, 9};
};
