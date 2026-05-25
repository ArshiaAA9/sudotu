#include <SFML/Window.hpp>
#include <iostream>
#include <print>
#include <random>

#include "game.hpp"

int main() {
    int seed;
    std::print("enter seed:");

    std::cin >> seed;
    std::random_device r{};

    Game game{seed};
    game.setValue(0, 0, 2);
    game.setValue(3, 1, 2);
    game.setValue(6, 2, 2);

    game.setValue(0, 8, 3);
    game.setValue(3, 7, 3);
    game.setValue(6, 6, 3);
    game.printBoard();
}
