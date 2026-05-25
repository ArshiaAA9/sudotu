#pragma once
#include <bit>
#include <iostream>
#include <iterator>
#include <print>
#include <random>
#include <string>

#include "types.hpp"

class Game {
public:
    Game() {
        std::random_device rd;
        int randomValue = rd();
        m_randEngine.seed(randomValue);
        std::print("{}\n", randomValue);
        generateSudoku();
    };

    explicit Game(int seed) {
        m_randEngine.seed(seed);
        std::print("entered seed: {}\n", seed);
        generateSudoku();
    }

    // tries to insert a value to a cell if sudoku rules allow it
    // if the cell is currently occupied it changes it
    bool setValue(unsigned int col, unsigned int row, int value);

    void generateSudoku();
    void printBoard() const;
    void clearBoard();
    int readValue(unsigned int col, unsigned row) const;
    bool isValid(unsigned int col, unsigned int row, int value) const;

private:
    int genValue();
    bool fillBoard(unsigned int col, unsigned int row);

    BoardType m_board;
    std::mt19937 m_randEngine;
    std::uniform_int_distribution<int> m_uniformDist{1, 9};
};
