#pragma once
#include <fstream>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <vector>

#include "../sudoku.hpp"

namespace ft = ftxui;

// --- constants ---
// constexpr unsigned int DEFAULT_DIFFICULTY = 39;
constexpr unsigned int DEFAULT_DIFFICULTY = 5;
constexpr std::string DEBUG_FILE_PATH = "debug.log";

// the game starts with the center selected (5 - 1,5 - 1)
constexpr unsigned int CENTER_COL = 4;
constexpr unsigned int CENTER_ROW = 4;

enum class Movement { UP, DOWN, LEFT, RIGHT };

class Tui {
public:
    // --- constructors ---
    explicit Tui(Sudoku::Sudoku& game)
        : m_game(game)
        , m_board(game.board())
        , m_difficulty(DEFAULT_DIFFICULTY)
        , m_logStream(std::ofstream(DEBUG_FILE_PATH))
        , m_screen(ft::ScreenInteractive::Fullscreen()) {
        newGame();
    }

    ~Tui() { m_logStream.close(); }

    void run();

private:
    // --- methods ---
    void newGame();
    bool handleEvents(const ft::Event& event);
    void move(Movement direction);
    void handlePlayerInput(unsigned int input);
    [[nodiscard]] ft::Element createMainDom();
    [[nodiscard]] ft::Element createSubGrid(unsigned int startingCol, unsigned int startingRow);
    [[nodiscard]] std::vector<ft::Elements> fillGridWithGrid();

    // --- members ---
    Sudoku::Sudoku& m_game;
    const Sudoku::Board& m_board;
    unsigned int m_difficulty;

    std::ofstream m_logStream; // not used currently, might use later
    ft::ScreenInteractive m_screen;
    Sudoku::Cell m_selected;
};
