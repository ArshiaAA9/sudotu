#pragma once
#include <condition_variable>
#include <fstream>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <vector>

#include "../sudoku.hpp"
#include "stopwatch.hpp"

namespace ft = ftxui;

// --- constants ---
constexpr unsigned int DEFAULT_DIFFICULTY = 45;
// constexpr unsigned int DEFAULT_DIFFICULTY = 5;
constexpr std::string DEBUG_FILE_PATH = "debug.log";

// the game starts with the center selected (5 - 1,5 - 1)
constexpr unsigned int CENTER_COL = 4;
constexpr unsigned int CENTER_ROW = 4;

enum class Movement { UP, DOWN, LEFT, RIGHT };

class Tui {
public:
    // --- constructors ---
    explicit Tui(Sudoku::Sudoku& game);

    ~Tui() {
        m_logStream.close();
        system("reset"); // fixes a cursor bug
    }

    void run();

private:
    // --- methods ---
    void newGame();
    // timerLoop runs on a different thread
    void timerLoop(); // used to force update ftxui's ui every second
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

    Stopwatch m_stopwatch;
    std::atomic<bool> m_isRunning = false;
    std::mutex m_mutex; // not used directly but needed for std::conditional_variable
    std::condition_variable m_cv;
    std::ofstream m_logStream; // not used currently, might use later

    ft::ScreenInteractive m_screen;
    // m_selected is set in the newGame() method
    Sudoku::Cell m_selected;
};
