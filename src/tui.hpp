#pragma once
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/string.hpp>
#include <string>

#include "types.hpp"
using namespace ftxui;

class Tui {
public:
    Tui(const BoardType& board)
        : m_board(board) {}

    void render(const BoardType& board) {
        auto document = border(vbox({
            text("Sudoku") | bold | center,
            separator(),
            hbox({
                // change these later
                text("Score: 0") | flex,
                text("Time: 0:00"),
            }),
            separator(),
            filler(),
            hbox({
                filler(),
                gridbox(fillGridCells()) | center | border | bgcolor(Color::Black),
                filler(),
            }),
            filler(),
        }));
        auto screen = Screen::Create(Dimension::Full());
        Render(screen, document);
        screen.Print();
    }

    std::vector<std::vector<Element>> fillGridCells() {
        std::vector<Elements> result;
        result.reserve(81);
        for (size_t row = 0; row < 9; row++) {
            Elements currentElementRow;
            for (size_t col = 0; col < 9; col++) {
                if (m_board[col][row] == 0) {
                    currentElementRow.push_back(
                        hbox({text("") | center | border | size(WIDTH, EQUAL, 5), filler() | size(WIDTH, EQUAL, 1)}));
                } else {
                    currentElementRow.push_back(hbox(
                        {text(std::to_string(m_board[col][row])) | center | border | size(WIDTH, EQUAL, 5),
                         filler() | size(WIDTH, EQUAL, 1)}));
                }
                if ((col + 1) % 3 == 0 && col != 8) {
                    currentElementRow.push_back(hbox({
                        separatorHeavy() | color(Color::Yellow),
                        filler() | size(WIDTH, EQUAL, 1),
                    }));
                }
            }
            result.push_back(currentElementRow);
            if ((row + 1) % 3 == 0 && row != 8) {
                Elements separatorRow;
                separatorRow.reserve(11);
                for (int i = 0; i < 11; i++) { // how many elements wide is your row?
                    separatorRow.push_back(separatorHeavy() | color(Color::Yellow));
                }
                result.push_back(separatorRow);
            }
        }
        return result;
    }

    const BoardType& m_board;
};
