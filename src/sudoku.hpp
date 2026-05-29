#pragma once
#include <array>
#include <cstdint>
#include <random>
#include <unordered_map>

namespace Sudoku {

constexpr int GRID_SIZE = 9;
constexpr int SUBGRID_SIZE = 3;

using Board = std::array<std::array<unsigned int, GRID_SIZE>, GRID_SIZE>;

struct Cell {
    Cell() = default;

    Cell(unsigned int col, unsigned int row)
        : col(col)
        , row(row) {}

    Cell(unsigned int col, unsigned int row, unsigned int value)
        : col(col)
        , row(row)
        , value(value) {}

    // position
    unsigned int col, row;
    unsigned int value;
};

enum class MoveResult {
    NOT_EMPTY,
    WIN,
    MISTAKE,
    SUCCEED,
};

class Sudoku {
public:
    Sudoku() = default;
    ~Sudoku() = default;

    // --- main apis ---
    void generateSudoku(unsigned int difficulty);
    void generateSudoku(unsigned int difficulty, uint32_t seed);
    void clearBoards();
    void printBoard() const;
    bool isValid(unsigned int col, unsigned int row, int value) const;
    int readValue(unsigned int col, unsigned row) const;

    // used to insert player move
    MoveResult insertValue(unsigned int col, unsigned int row, unsigned int value);

    // should be used for board setups and testing only.
    // returns on success
    bool setValue(unsigned int col, unsigned int row, unsigned int value);

    // --- getters ---
    const Board& board() const;

    unsigned int mistakeCount() const;

    const std::unordered_map<unsigned int, Cell>& mistakes() const;

private:
    // --- methods ---
    bool isInBound(unsigned int col, unsigned int row, unsigned int value);
    bool fillBoard(unsigned int col, unsigned int row);
    void punchHoles(unsigned int difficulty);
    bool hasOneSolution();
    void solveForSolutions(unsigned int col, unsigned int row, unsigned int& uniqueSolutionCount);

    // --- members ---
    Board m_board{};
    // used to check answers against it
    Board m_filledBoard;
    std::mt19937 m_randEngine;
    uint32_t m_currentSeed;
    unsigned int m_emptyCells;
    unsigned int m_mistakesCount = 0;
    // the mistake number is the key
    std::unordered_map<unsigned int, Cell> m_mistakes;
};
} // namespace Sudoku
