#pragma once
#include <array>
#include <catch2/catch_test_macros.hpp>

const int GRID_SIZE = 9;
const int BLOCK_SIZE = 3;

using BoardType = std::array<std::array<unsigned int, GRID_SIZE>, GRID_SIZE>;

struct Mistake {
    Mistake(unsigned int col, unsigned int row, unsigned int value)
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
