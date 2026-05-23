
#include <SFML/Window.hpp>
#include <print>
#include <string>

#include "game.hpp"

/*
place a random number at the start.
on the next cell place another random number
check if the number is valid based on sudoku rules
if yes keep going
if not try a different value for current cells
if nothing out of 1-9 go back a cell and
try different values from there
if nothing 1-9 fit go back another one
*/

// recursive function used to generate a board
bool fillBoard() {}

int main() {
    Game game{};
    game.insertValue(0, 0, 2);
    game.insertValue(3, 1, 2);
    game.insertValue(6, 2, 2);

    game.insertValue(0, 8, 3);
    game.insertValue(3, 7, 3);
    game.insertValue(6, 6, 3);
    game.printBoard();
}
