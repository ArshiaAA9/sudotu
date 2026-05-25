#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "game.hpp"
#include "types.hpp"

TEST_CASE("validating values", "[isValid]") {
    Game game{};
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

    Game game{};
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
