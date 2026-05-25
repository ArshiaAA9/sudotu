#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "sudoku.hpp"
#include "types.hpp"

TEST_CASE("validating values", "[isValid]") {
    Sudoku game{};
    game.clearBoard();

    // test
    game.setValue(0, 0, 2);
    game.setValue(3, 1, 2);

    game.setValue(0, 8, 3);
    game.setValue(3, 7, 3);

    SECTION("vertical test") {
        REQUIRE(game.setValue(3, 5, 3) == false);
        REQUIRE(game.setValue(4, 5, 3) == true);
    }

    SECTION("horizontal test") {
        REQUIRE(game.setValue(6, 1, 2) == false);
        REQUIRE(game.setValue(6, 2, 2) == true);
    }

    SECTION("in the same block test") {
        REQUIRE(game.setValue(2, 2, 2) == false);
        REQUIRE(game.setValue(2, 2, 4) == true);
    }
}

// goes over the board and checks if all are correct
TEST_CASE("generating full board", "[fillBoard]") {
    Sudoku game{};
    // pass the difficulty as 0 so there are no empty cells
    game.generateSudoku(0);
    game.printBoard();
    int currentValue;
    for (size_t x = 0; x < GRID_SIZE; x++) {
        for (size_t y = 0; y < GRID_SIZE; y++) {
            currentValue = game.readValue(x, y);
            REQUIRE(currentValue != 0);
            REQUIRE(game.isValid(x, y, currentValue));
        }
    }
}

TEST_CASE("seed consistancy") {

    SECTION("same seed produces identical boards") {
        Sudoku game1{};
        Sudoku game2{};
        game1.generateSudoku(10, 123);
        game2.generateSudoku(10, 123);
        REQUIRE(game1.board() == game2.board());
        game1.generateSudoku(10, 321);
        game2.generateSudoku(10, 321);
        REQUIRE(game1.board() == game2.board());
    }
    SECTION("same seed produces identicaly board after random generation") {
        Sudoku game1{};
        Sudoku game2{};
        game1.generateSudoku(10);
        game2.generateSudoku(10);
        REQUIRE(game1.board() != game2.board());
        game1.generateSudoku(10, 444);
        game2.generateSudoku(10, 444);
        REQUIRE(game1.board() == game2.board());
    }
}

TEST_CASE("hole punching", "[punchHoles)]") {
    SECTION("hole amounts match difficulty") {
        unsigned int difficulty = 10;
        Sudoku game{};
        game.generateSudoku(difficulty);
        BoardType board = game.board();
        unsigned int emptyValues{};
        for (size_t col = 0; col < GRID_SIZE; col++) {
            for (size_t row = 0; row < GRID_SIZE; row++) {
                if (board[col][row] == 0) emptyValues++;
            }
        }
        REQUIRE(emptyValues == difficulty);
    }
}
