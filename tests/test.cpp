#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "game.hpp"

TEST_CASE("validating values", "[isValid]") {
    Game game{};

    // test
    game.insertValue(0, 0, 2);
    game.insertValue(3, 1, 2);

    game.insertValue(0, 8, 3);
    game.insertValue(3, 7, 3);

    SECTION("vertical test") {
        REQUIRE(game.insertValue(3, 5, 3) == false);
        REQUIRE(game.insertValue(4, 5, 3) == true);
    }

    SECTION("horizontal test") {
        REQUIRE(game.insertValue(6, 1, 2) == false);
        REQUIRE(game.insertValue(6, 2, 2) == true);
    }

    SECTION("in the same block test") {
        REQUIRE(game.insertValue(2, 2, 2) == false);
        REQUIRE(game.insertValue(2, 2, 4) == true);
    }
}
