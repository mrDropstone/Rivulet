#pragma once
#include"line.hpp"
#include"appearance.hpp"
#include"screen_size.hpp"


namespace Rivulet {
template<typename LineContainer=Line<line<u32_char>>>
class Display {
private:
    bool screen_empty = true;
    bool is_cleared = false;
    struct winsize old;

public:
    std::vector<LineContainer> display;
    Appearance appearance;
    unsigned int row = 0;
    unsigned int col = 0;
    ScreenSizeGetter size;

private:
    void prevent_overflow(unsigned int _row) {
        if(_row > 1000) return;
        if(_row >= display.size()) {
            int times = _row - display.size() + 1;
            display.reserve(times);
            for(int i = 0; i < times; i++) {
                display.emplace_back();
            }
        }
    }
    void prevent_overflow() { prevent_overflow(row); }

public:
    Display() {}
    void overwrite(char c) {
        prevent_overflow();
        display.at(row)._overwrite(c, col, appearance);
    }
    void clear() {
        for(LineContainer& line : display) {
            line.clear();
        }
    }
    LineContainer& line(unsigned int _row) {
        prevent_overflow(_row);
        return display[_row];
    }
    void clear_line(unsigned int _row) {
        if(display.size() > _row) display.at(_row).clear();
    }
    void overwrite(const char* chars) {
        prevent_overflow();
        display.at(row)._overwrite(chars, col, appearance);
    }
    void set(const char* chars) {
        prevent_overflow();
        unsigned int no;
        display.at(row)._overwrite(chars, no, appearance);
    }
    Display& nth(unsigned int _row, unsigned int _col) {
        row = _row;
        col = _col;
        return *this;
    }
    Display& bg(uint8_t red, uint8_t green, uint8_t blue) {
        appearance.bg.red = red; appearance.bg.green = green; appearance.bg.blue = blue;
        appearance.bg.is_default_color = false;
        return *this;
    }
    Display& fg(uint8_t red, uint8_t green, uint8_t blue) {
        appearance.fg.red = red; appearance.fg.green = green; appearance.fg.blue = blue;
        appearance.fg.is_default_color = false;
        return *this;
    }
    Display& bg(uint8_t default_color) {
        appearance.bg.red = default_color;
        appearance.bg.is_default_color = true;
        return *this;
    }
    Display& fg(uint8_t default_color) {
        appearance.fg.red = default_color;
        appearance.fg.is_default_color = true;
        return *this;
    }


    Display& bold(bool state = true) {
        appearance.style.bold = state;
        if(!state) appearance.style.dim = false;
        return *this;
    }
    Display& dim(bool state = true) {
        appearance.style.dim = state;
        if(!state) appearance.style.bold = false;
        return *this;
    }
    Display& italic(bool state = true) {
        appearance.style.italic = state;
        return *this;
    }
    Display& underline(bool state) {
        appearance.style.underline = state;
        return *this;
    }
    Display& blink(bool state = true) {
        appearance.style.blink = state;
        return *this;
    }
    Display& reverse(bool state = true) {
        appearance.style.reverse = state;
        return *this;
    }
    Display& invisible(bool state = true) {
        appearance.style.invisible = state;
        return *this;
    }
    Display& strike(bool state = true) {
        appearance.style.strike = state;
        return *this;
    }
    Display& default_appearance() {
        appearance.style.bold = false;
        appearance.style.dim = false;
        appearance.style.bold = false;
        appearance.style.italic = false;
        appearance.style.blink = false;
        appearance.style.reverse = false;
        appearance.style.invisible = false;
        appearance.style.strike = false;
        return *this;
    }
    void render_on_empty_screen() {
        size.update();
        std::cout << "\r\n";
        for(int i = 0; i < display.size(); i++) {
            display.at(i).fill(size.current_ws.ws_col);
            std::cout << "\r\n";
            display.at(i).render(size.current_ws.ws_col, i);
            continue;
        }
        screen_empty = false;
    }
    void render() {
        if(screen_empty) render_on_empty_screen();
        for(int i = 0; i < display.size(); i++) {
            display.at(i).render(size.current_ws.ws_col, i);
        }
    }
    template<typename DisplayTemplateArgument>
    friend class GridCore;
};

}
