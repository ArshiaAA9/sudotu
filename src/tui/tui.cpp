#include "tui.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <string>
using namespace Sudoku;

// --------------------------PUBLICS--------------------------

void Tui::run() {
    auto renderer = ft::Renderer([&]() { return createMainDom(); });
    auto component = CatchEvent(renderer, [&](const ft::Event event) { return handleEvents(event); });
    m_screen.Loop(component);
}

// --------------------------PRIVATES--------------------------

void Tui::newGame() {
    m_game.generateSudoku(m_difficulty);
    m_selected.col = CENTER_COL;
    m_selected.row = CENTER_ROW;
    m_selected.value = m_board[CENTER_COL][CENTER_ROW];
}

bool Tui::handleEvents(const ft::Event& event) {
    if (event == ft::Event::q) {
        m_screen.ExitLoopClosure()();
        return true;
    }
    if (event == ft::Event::ArrowUp || event == ft::Event::w) {
        move(Movement::UP);
        return true;
    }
    if (event == ft::Event::ArrowDown || event == ft::Event::s) {
        move(Movement::DOWN);
        return true;
    }
    if (event == ft::Event::ArrowLeft || event == ft::Event::a) {
        move(Movement::LEFT);
        return true;
    }
    if (event == ft::Event::ArrowRight || event == ft::Event::d) {
        move(Movement::RIGHT);
        return true;
    }
    if (event.is_character() && event.character().size() == 1) { // check size to skip multi-byte cases
        const char ch = event.character().at(0);
        // every character has an int corresponding number we use that to check here
        if (ch >= '1' && ch <= '9') {
            m_logStream << ch << " was pressed\n";
            m_logStream.flush();
            handlePlayerInput(ch - '0');
            return true;
        }
        return true;
    }
    return false;
}

void Tui::move(Movement direction) {
    switch (direction) {
        case Movement::UP:
            if (m_selected.row == 0) break;
            // remember negative row is up
            m_selected.row--;
            break;
        case Movement::DOWN:
            if (m_selected.row == GRID_SIZE - 1) break;
            m_selected.row++;
            break;
        case Movement::LEFT:
            if (m_selected.col == 0) break;
            m_selected.col--;
            break;
        case Movement::RIGHT:
            if (m_selected.col == GRID_SIZE - 1) break;
            m_selected.col++;
            break;
    }
}

void Tui::handlePlayerInput(unsigned int input) {
    MoveResult moveRes = m_game.insertValue(m_selected.col, m_selected.row, input);
    switch (moveRes) {
        case MoveResult::NOT_EMPTY:
            // do nothing for now
            break;
        case MoveResult::SUCCEED:
            // TODO: add different color for player inputed things
            break;
        case MoveResult::WIN:
            // TODO: prompt user if they want play again.
            newGame();
            break;
        case MoveResult::MISTAKE:

            break;
    }
}

ft::Element Tui::createMainDom() {
    auto document = border(
        ft::vbox({
            ft::text("Sudoku") | ft::bold | ft::center,
            ft::separator(),
            ft::hbox({
                // change these later
                ft::text("Mistakes: " + std::to_string(m_game.mistakeCount())) | ft::flex,
                ft::text("Time: 0:00"),
            }),
            ft::separator(),
            ft::filler(),
            ft::hbox({
                ft::filler(),
                gridbox(fillGridWithGrid()) | ft::center | ft::bgcolor(ft::Color::White),
                ft::filler(),
            }),
            ft::filler(),
        }));
    return document;
}

ft::Element Tui::createSubGrid(unsigned int startingCol, unsigned int startingRow) {
    std::vector<ft::Elements> result;

    constexpr int CELL_WIDTH = 7;
    constexpr int CELL_HEIGHT = 3;
    // constexpr int FILLER_WIDTH = 1;

    // const ft::Element filler = ft::filler() | size(ft::WIDTH, ft::EQUAL, 4) | size(ft::HEIGHT, ft::EQUAL, 2);

    for (size_t row = startingRow * SUBGRID_SIZE; row < startingRow * SUBGRID_SIZE + SUBGRID_SIZE; row++) {
        ft::Elements currentRow;
        for (size_t col = startingCol * SUBGRID_SIZE; col < startingCol * SUBGRID_SIZE + SUBGRID_SIZE; col++) {
            ft::Element text;
            if (col == m_selected.col && row == m_selected.row) {
                // selected case
                // "\u25CB" unicode for "○"
                text = (m_board[col][row] == 0) ? ft::text("○") : ft::text(std::to_string(m_board[col][row]));
                text |= ft::color(ft::Color::Red);

            } else {
                text = (m_board[col][row] == 0) ? ft::text("") : ft::text(std::to_string(m_board[col][row]));
                text |= ft::color(ft::Color::Black);
            }
            text |= ft::center;
            text |= ft::size(ft::WIDTH, ft::EQUAL, CELL_WIDTH);
            text |= ft::size(ft::HEIGHT, ft::EQUAL, CELL_HEIGHT);

            currentRow.push_back(text);

            if (col < startingCol * SUBGRID_SIZE + 2)
                currentRow.push_back(ft::separatorLight() | color(ft::Color::Black));
        }
        result.push_back(currentRow);

        if (row < startingRow * 3 + 2) {
            ft::Elements separatorRow;
            separatorRow.reserve(6);
            for (int i = 0; i < 5; i++) {
                separatorRow.push_back(ft::separatorLight() | color(ft::Color::Black));
            }
            result.push_back(separatorRow);
        }
    }

    return ft::gridbox(result);
}

std::vector<ft::Elements> Tui::fillGridWithGrid() {
    std::vector<ft::Elements> result;

    constexpr int BLOCK_WIDTH = 23;
    constexpr int BLOCK_HEIGHT = 11;

    // const ft::Element filler = ft::filler() | size(ft::HEIGHT, ft::EQUAL, FILLER_WIDTH);

    for (size_t blockRow = 0; blockRow < SUBGRID_SIZE; blockRow++) {
        ft::Elements currentRow;
        for (size_t blockCol = 0; blockCol < SUBGRID_SIZE; blockCol++) {
            ft::Element currentBlock = createSubGrid(blockCol, blockRow);
            currentBlock |= ft::size(ft::WIDTH, ft::EQUAL, BLOCK_WIDTH);
            currentBlock |= ft::size(ft::HEIGHT, ft::EQUAL, BLOCK_HEIGHT);

            // currentBlock |= ft::bgcolor(ft::Color(ftxui::Color::Magenta));

            currentRow.push_back(currentBlock);

            if (blockCol < 2) {
                currentRow.push_back(ft::separatorHeavy() | color(ft::Color::Black));
            }
        }
        result.push_back(currentRow);

        if (blockRow < 2) {
            ft::Elements separatorRow;
            separatorRow.reserve(10);
            if (blockRow != 2) {
                for (int i = 0; i < 5; i++) {
                    separatorRow.push_back(ft::separatorHeavy() | color(ft::Color::Black));
                }
                result.push_back(separatorRow);
            }
        }
    }
    return result;
}
