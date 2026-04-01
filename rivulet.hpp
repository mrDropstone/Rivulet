#pragma once
#include<stdint.h>
#include<termios.h>
#include<unistd.h>
#include<iostream>
#include"display.hpp"
#include"key_detect.hpp"
#include"cursor.hpp"


class TerminalConfiguration {
private:
    struct termios original_termios;
    struct termios current_termios;
public:
    TerminalConfiguration() {
        std::cout << "\033[?1049h\x1b[?7l";
        tcgetattr(STDIN_FILENO, &original_termios);
        current_termios = original_termios;
        current_termios.c_lflag &= ~(ECHO | ICANON | IEXTEN);
        current_termios.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
        current_termios.c_oflag &= ~(OPOST);
        current_termios.c_cflag |= (CS8);
        current_termios.c_cc[VMIN] = 0;
        current_termios.c_cc[VTIME] = 0;
        if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &current_termios) == -1) { std::cout << "Error loading configuration." << std::endl; std::exit(1); }
    }
    ~TerminalConfiguration() {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
        std::cout << "\033[?1049l\x1b[?7h";
    }
};

template<typename DisplayTemplateArgument>
class RivuletCore {
public:
    Cursor cursor;
    Display<DisplayTemplateArgument> display;
    KeyDetection key;
    TerminalConfiguration config;
public:
    void render() {
        display.render();
        cursor.render();
        std::cout << std::flush;
    }
};

using Rivulet = RivuletCore<Line<line<u32_char>>>;
