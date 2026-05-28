#pragma once
#include <ftxui/component/component.hpp>

#include "../types.hpp"
namespace ft = ftxui;

enum class Movement { UP, DOWN, LEFT, RIGHT };

class Tui {
public:
    Tui() {}

    void run(const BoardType& board);

private:
    void move(Movement move);
    ft::Element createMainDom(const BoardType& board);
    std::vector<std::vector<ft::Element>> fillGridCells(const BoardType& board);
    // center is (4,4)
    unsigned int m_selectedCol = 4;
    unsigned int m_selectedRow = 4;
};
