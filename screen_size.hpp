#pragma once
#include<sys/ioctl.h>
#include<termios.h>
#include<unistd.h>

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
