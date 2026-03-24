#pragma once
#include"line.hpp"
struct ScreenSize {
    unsigned int rows;
    unsigned int cols;
    unsigned int pixels_x;
    unsigned int pixels_y;
    ScreenSize(unsigned int rows, unsigned int cols, unsigned int pixels_x, unsigned int pixels_y) : 
        rows(rows), cols(cols), pixels_x(pixels_x), pixels_y(pixels_y) {}
    bool operator==(const ScreenSize& other) {
        return other.rows == rows && other.cols == cols && other.pixels_x == pixels_x && other.pixels_y == pixels_y;
    }
    bool operator!=(const ScreenSize& other) {
        return !operator==(other);
    }
};

class ScreenSizeGetter {
private:
    struct winsize current_ws;
    void update() {
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &current_ws);
    }

public:
    unsigned int cols() {
        update();
        return current_ws.ws_col;
    }   
    unsigned int rows() {
        update();
        return current_ws.ws_row;
    }
    ScreenSize get_screen() {
        update();
        return ScreenSize(current_ws.ws_row, current_ws.ws_col, current_ws.ws_xpixel, current_ws.ws_ypixel);
    }
    winsize get_winsize() {
        update();
        return current_ws;
    }
    ScreenSize operator()() {
        update();
        return ScreenSize(current_ws.ws_row, current_ws.ws_col, current_ws.ws_xpixel, current_ws.ws_ypixel);
    }
    template<typename LineContainer>
    friend class Display;
};

template<typename LineContainer=Line<line<u32_char>>>
class Display {
private:
    bool screen_empty = true;
    bool is_cleared = false;
    struct winsize old;

public:
    std::vector<LineContainer> display;
    Style style;
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
    Display()  {
    }
    void overwrite(char c) {
        prevent_overflow();
        display.at(row)._overwrite(c, col, style);
    }
    void clear() {
        for(LineContainer& line : display) {
            line.clear();
            //display[i].mark_unmodified();
        }
        //is_cleared = true;
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
        display.at(row)._overwrite(chars, col, style);
    }
    void set(const char* chars) {
        prevent_overflow();
        unsigned int no;
        display.at(row)._overwrite(chars, no, style);
    }
    Display& nth(unsigned int _row, unsigned int _col) {
        row = _row;
        col = _col;
        return *this;
    }
    Display& bg(uint8_t red, uint8_t green, uint8_t blue) {
        style.bg.red = red; style.bg.green = green; style.bg.blue = blue;
        style.bg.is_default_color = false;
        return *this;
    }
    Display& fg(uint8_t red, uint8_t green, uint8_t blue) {
        style.fg.red = red; style.fg.green = green; style.fg.blue = blue;
        style.fg.is_default_color = false;
        return *this;
    }
    Display& bg(uint8_t default_color) {
        style.bg.red = default_color;
        style.bg.is_default_color = true;
        return *this;
    }
    Display& fg(uint8_t default_color) {
        style.fg.red = default_color;
        style.fg.is_default_color = true;
        return *this;
    }


    Display& bold(bool state = true) {
        style.bold = state;
        if(!state) style.dim = false;
        return *this;
    }
    Display& dim(bool state = true) {
        style.dim = state;
        if(!state) style.bold = false;
        return *this;
    }
    Display& italic(bool state = true) {
        style.italic = state;
        return *this;
    }
    Display& underline(bool state) {
        style.underline = state;
        return *this;
    }
    Display& blink(bool state = true) {
        style.blink = state;
        return *this;
    }
    Display& reverse(bool state = true) {
        style.reverse = state;
        return *this;
    }
    Display& invisible(bool state = true) {
        style.invisible = state;
        return *this;
    }
    Display& strike(bool state = true) {
        style.strike = state;
        return *this;
    }
    Display& default_style() {
        style.bold = false;
        style.dim = false;
        style.bold = false;
        style.italic = false;
        style.blink = false;
        style.reverse = false;
        style.invisible = false;
        style.strike = false;
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
    friend class Rivulet;
};


