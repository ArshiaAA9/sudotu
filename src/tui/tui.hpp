#pragma once
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <vector>

#include "../types.hpp"
namespace ft = ftxui;

enum class Movement { UP, DOWN, LEFT, RIGHT };

class Tui {
public:
    Tui() {}

    void test() {
        using namespace ftxui;
        auto cell = [](const char* t) { return text(t) | border; };
        auto document = //
            gridbox({
                {
                    cell("north-west"),
                    cell("north"),
                    cell("north-east"),
                },
                {
                    cell("center-west"),
                    gridbox({
                        {
                            cell("center-north-west"),
                            cell("center-north-east"),
                        },
                        {
                            cell("center-south-west"),
                            cell("center-south-east"),
                        },
                    }),
                    cell("center-east"),
                },
                {
                    cell("south-west"),
                    cell("south"),
                    cell("south-east"),
                },
            });
        auto screen = Screen::Create(Dimension::Fit(document));
        Render(screen, document);
        screen.Print();
        getchar();
    }

    void run(const BoardType& board);

private:
    void move(Movement move);
    ft::Element createMainDom(const BoardType& board);
    std::vector<ft::Elements> fillGridWithGrid(const BoardType& board);
    ft::Element createGridBox(const BoardType& board, unsigned int startingCol, unsigned int startingRow);
    std::vector<ft::Elements> fillGridCells(const BoardType& board);
    // center is (4,4)
    unsigned int m_selectedCol = 4;
    unsigned int m_selectedRow = 4;
};
