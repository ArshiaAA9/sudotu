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
                gridbox(fillGridCells(board)) | ft::center | ft::borderStyled(ft::Color::Yellow),
                ft::filler(),
            }),
            ft::filler(),
        }));
    return document;
}

std::vector<std::vector<ft::Element>> Tui::fillGridCells(const BoardType& board) {
    std::vector<ft::Elements> result;
    result.reserve(81);

    constexpr int CELL_WIDTH = 3;
    constexpr int CELL_HEIGHT = 1;
    constexpr int FILLER_WIDTH = 1;

    const ft::Element filler = ft::filler() | size(ft::HEIGHT, ft::EQUAL, FILLER_WIDTH);

    for (size_t row = 0; row < GRID_SIZE; row++) {
        ft::Elements currentRow;
        for (size_t col = 0; col < GRID_SIZE; col++) {
            ft::Element text = (board[col][row] == 0) ? ft::text(" ") : ft::text(std::to_string(board[col][row]));

            text |= ft::center;
            text |= size(ft::WIDTH, ft::EQUAL, CELL_WIDTH);
            text |= size(ft::HEIGHT, ft::EQUAL, CELL_HEIGHT);

            if (col == m_selectedCol && row == m_selectedRow) text |= ft::bgcolor(ft::Color::Cyan);
            // text |= ft::borderHeavy;

            const ft::Element current = hbox(text, filler);
            // const ft::Element current = hbox(text);
            currentRow.push_back(current);
            if ((col + 1) % 3 == 0 && col != 8) {
                currentRow.push_back(ft::hbox({ft::separatorHeavy() | color(ft::Color::Yellow), filler}));
            }
        }
        result.push_back(currentRow);
        if ((row + 1) % 3 == 0 && row != 8) {
            ft::Elements separatorRow;
            // 11 is the element count 9 rows + 2 filler
            separatorRow.reserve(11);
            for (int i = 0; i < 11; i++) {
                separatorRow.push_back(ft::separatorHeavy() | color(ft::Color::Yellow));
            }
            result.push_back(separatorRow);
        }
    }
    return result;
}
