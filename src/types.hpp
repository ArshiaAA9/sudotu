#pragma once
#include <array>

const int GRID_SIZE = 9;
const int BLOCK_SIZE = 3;

using BoardType = std::array<std::array<unsigned int, GRID_SIZE>, GRID_SIZE>;
