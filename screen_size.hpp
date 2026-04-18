#pragma once
#include<sys/ioctl.h>
#include<termios.h>
#include<unistd.h>

namespace Rivulet {
struct ScreenSize {
    int cols;
    int rows;
    int pixels_x;
    int pixels_y;
    ScreenSize() {
        update();
    }
    ScreenSize(const ScreenSize& other) {
        cols = other.cols;
        rows = other.rows;
        pixels_x = other.pixels_x;
        pixels_y = other.pixels_y;
    }
    bool operator==(const ScreenSize& other) {
        return other.cols == cols && other.rows == rows;
    }
    bool operator!=(const ScreenSize& other) {
        return !operator==(other);
    }
    void update() {
        struct winsize current_ws;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &current_ws);
        cols = current_ws.ws_col;
        rows = current_ws.ws_row;
        pixels_x = current_ws.ws_xpixel;
        pixels_y = current_ws.ws_ypixel;
    }
    template<typename LineContainer>
    friend class Display;
};
}
