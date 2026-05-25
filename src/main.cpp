#include <cstdint>
#include <iostream>
#include <print>
#include <string>

#include "sudoku.hpp"
#include "tui.hpp"

void play(Sudoku& game) {
    std::string answer;
    while (true) {
        std::print("Do you want to enter a seed? (y/N): ");
        std::getline(std::cin, answer);
        if (answer.empty()) continue; // ignore empty line
        char first = std::tolower(static_cast<unsigned char>(answer[0]));
        if (first == 'y') {
            uint32_t seed = 0;
            bool valid = false;
            while (!valid) {
                std::print("Enter a seed (uint32_t): ");
                std::string line;
                std::getline(std::cin, line);

                // Parse using std::from_chars (fast and precise)
                auto [ptr, ec] = std::from_chars(line.data(), line.data() + line.size(), seed);
                if (ec == std::errc() && ptr == line.data() + line.size()) {
                    valid = true;
                } else {
                    std::println("Not a valid number. Please enter an unsigned 32‑bit integer.");
                }
            }
            game.generateSudoku(5, seed);
            game.printBoard();
            return;
        } else if (first == 'n' || answer == "N" || answer.empty()) {
            game.generateSudoku(5); // default seed
            game.printBoard();
            return;
        } else {
            std::println("Please answer 'y' or 'N'.");
        }
    }
}

int main() {
    Tui tui{};
    tui.render();
    Sudoku game{};
    bool playing = true;
    while (playing) {
        play(game);
        std::string answer;
        while (true) {
            std::print("Do you want to play again? (y/N): ");
            std::getline(std::cin, answer);
            if (answer.empty()) continue;
            char first = std::tolower(static_cast<unsigned char>(answer[0]));
            if (first == 'y') {
                break; // outer loop continues
            } else if (first == 'n') {
                playing = false;
                break;
            } else {
                std::println("Please answer 'y' or 'N'.");
            }
        }
    }
    std::println("Goodbye!");
}
