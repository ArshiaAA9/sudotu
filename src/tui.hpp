#pragma once
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
using namespace ftxui;

class Tui {
public:
    Tui() {}

    void render() {
        auto document = border(vbox(text("Hello Sudoku!"), text("world"), text(" sonaugadgba"))) | color(Color::Red);
        auto screen = Screen::Create(Dimension::Full());
        Render(screen, document);
        screen.Print();
    }
};
