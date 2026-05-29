#include "tui.hpp"

#include <fstream>
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>

void Tui::run(const BoardType& board) {
    std::ofstream log("debug.log");

    auto screen = ft::ScreenInteractive::Fullscreen();
    auto renderer = ft::Renderer([&]() { return createMainDom(board); });
    auto component = CatchEvent(renderer, [&](ft::Event event) {
        if (event == ft::Event::q) {
            screen.ExitLoopClosure()();
            return true;

        } else if (event == ft::Event::ArrowUp || event == ft::Event::w) {
            move(Movement::UP);
            return true;

        } else if (event == ft::Event::ArrowDown || event == ft::Event::s) {
            move(Movement::DOWN);
            return true;

        } else if (event == ft::Event::ArrowLeft || event == ft::Event::a) {
            move(Movement::LEFT);
            return true;

        } else if (event == ft::Event::ArrowRight || event == ft::Event::d) {
            move(Movement::RIGHT);
            return true;
        }
        return false;
    });
    screen.Loop(component);
}

void Tui::move(Movement move) {
    switch (move) {
        case Movement::UP:
            if (m_selectedRow == 0) break;
            // remember negative row is up
            m_selectedRow--;
            break;
        case Movement::DOWN:
            if (m_selectedRow == GRID_SIZE - 1) break;
            m_selectedRow++;
            break;
        case Movement::LEFT:
            if (m_selectedCol == 0) break;
            m_selectedCol--;
            break;
        case Movement::RIGHT:
            if (m_selectedCol == GRID_SIZE - 1) break;
            m_selectedCol++;
            break;
    }
}

ft::Element Tui::createMainDom(const BoardType& board) {
    auto document = border(
        ft::vbox({
            ft::text("Sudoku") | ft::bold | ft::center,
            ft::separator(),
            ft::hbox({
                // change these later
                ft::text("Score: 0") | ft::flex,
                ft::text("Time: 0:00"),
            }),
            ft::separator(),
            ft::filler(),
            ft::hbox({
                ft::filler(),
                gridbox(fillGridWithGrid(board)) | ft::center | ft::borderStyled(ft::HEAVY, ft::Color::White),
                ft::filler(),
            }),
            ft::filler(),
        }));
    return document;
}

ft::Element Tui::createGridBox(const BoardType& board, unsigned int startingCol, unsigned int startingRow) {
    std::vector<ft::Elements> result;

    constexpr int CELL_WIDTH = 6;
    constexpr int CELL_HEIGHT = 2;
    constexpr int FILLER_WIDTH = 1;

    const ft::Element filler = ft::filler() | size(ft::WIDTH, ft::EQUAL, 4) | size(ft::HEIGHT, ft::EQUAL, 2);

    for (size_t row = startingRow * 3; row < startingRow * 3 + 3; row++) {
        ft::Elements currentRow;
        for (size_t col = startingCol * 3; col < startingCol * 3 + 3; col++) {
            ft::Element text =
                (board[col][row] == 0) ? ft::text("  ") : ft::text(" " + std::to_string(board[col][row]) + " ");
            text |= ft::center;
            text |= ft::size(ft::WIDTH, ft::GREATER_THAN, CELL_WIDTH);
            text |= ft::size(ft::HEIGHT, ft::GREATER_THAN, CELL_HEIGHT);

            if (col == m_selectedCol && row == m_selectedRow) text |= ft::bgcolor(ft::Color::Cyan);

            currentRow.push_back(text);

            if (col == startingCol * 3 + 2) continue;
            currentRow.push_back(ft::separatorLight() | color(ft::Color::White));
        }

        result.push_back(currentRow);
        ft::Elements separatorRow;
        separatorRow.reserve(6);
        for (int i = 0; i < 5; i++) {
            separatorRow.push_back(ft::separatorLight() | color(ft::Color::White));
        }
        result.push_back(separatorRow);
    }

    return gridbox(result);
}

std::vector<ft::Elements> Tui::fillGridWithGrid(const BoardType& board) {
    std::vector<ft::Elements> result;

    constexpr int CELL_WIDTH = 3;
    constexpr int CELL_HEIGHT = 1;
    constexpr int FILLER_WIDTH = 1;

    const ft::Element filler = ft::filler() | size(ft::HEIGHT, ft::EQUAL, FILLER_WIDTH);

    for (size_t blockRow = 0; blockRow < 3; blockRow++) {
        ft::Elements currentRow;
        for (size_t blockCol = 0; blockCol < 3; blockCol++) {
            ft::Element currentGrid = createGridBox(board, blockCol, blockRow);
            // currentGrid |= ft::center;
            currentGrid |= ft::size(ft::WIDTH, ft::EQUAL, 16);
            currentGrid |= ft::size(ft::HEIGHT, ft::EQUAL, 8);

            currentRow.push_back(currentGrid);
            if (blockCol != 2) {
                currentRow.push_back(ft::hbox({ft::separatorHeavy() | color(ft::Color::White), filler}));
            }
        }
        result.push_back(currentRow);
        ft::Elements separatorRow;
        // 11 is the element count 3 rows + 2 filler
        separatorRow.reserve(10);
        if (blockRow != 2) {
            for (int i = 0; i < 5; i++) {
                separatorRow.push_back(ft::separatorHeavy() | color(ft::Color::White));
            }
            result.push_back(separatorRow);
        }
    }
    return result;
}
