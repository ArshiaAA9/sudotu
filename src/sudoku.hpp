#pragma once
#include <cstdint>
#include <random>
#include <unordered_map>

#include "types.hpp"

class Sudoku {
public:
    Sudoku() {};

    // should be used for board setups and testing only.
    // returns on success
    bool setValue(unsigned int col, unsigned int row, unsigned int value);

    void generateSudoku(unsigned int difficulty);
    void generateSudoku(unsigned int difficulty, uint32_t seed);
    void clearBoards();
    void printBoard() const;

    bool isValid(unsigned int col, unsigned int row, int value) const;
    int readValue(unsigned int col, unsigned row) const;
    MoveResult insertValue(unsigned int col, unsigned int row, unsigned int value);

    inline const BoardType& board() const { return m_board; }

private:
    bool isInBound(unsigned int col, unsigned int row, unsigned int value);
    bool fillBoard(unsigned int col, unsigned int row);
    void punchHoles(unsigned int difficulty);
    bool hasOneSolution();
    void solveForSolutions(unsigned int col, unsigned int row, unsigned int& uniqueSolutionCount);

    BoardType m_board{};
    // used to check answers against it
    BoardType m_filledBoard;
    std::mt19937 m_randEngine;
    uint32_t m_currentSeed;
    unsigned int m_emptyCells;
    unsigned int m_mistakesCount = 0;
    // the mistake number is the key
    std::unordered_map<unsigned int, Mistake> m_mistakes;
};
