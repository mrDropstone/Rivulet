#pragma once
#include<stdint.h>
#include<vector>
#include<array>
#include<termios.h>
#include<unistd.h>
#include <sys/ioctl.h>
#include<iostream>
#include<stdexcept>
#include"display.hpp"

struct AnsiKeySequence {
    const unsigned char* sequence;
    int size;
    AnsiKeySequence(size_t size, const char* _sequence) : size(size), sequence((const unsigned char*)_sequence) {}
};

struct KeyData {
    AnsiKeySequence right_arrow{3, "\x1B\x5B\x43"};
    AnsiKeySequence left_arrow {3, "\x1B\x5B\x44"};
    AnsiKeySequence up_arrow{3, "\x1B\x5B\x41"};
    AnsiKeySequence down_arrow {3, "\x1B\x5B\x42"};
    unsigned char tab = '\10';
    unsigned char esc = '\x1B';
};
struct ShiftData {
    AnsiKeySequence left_arrow{6, "\033[1;2D"};
    AnsiKeySequence right_arrow{6, "\033[1;2C"};
    AnsiKeySequence up_arrow{6, "\033[1;2A"};
    AnsiKeySequence down_arrow{6, "\033[1;2B"};
    AnsiKeySequence tab{3, "\033[\0;\17"};
};
struct CtrlData {
    AnsiKeySequence left_arrow{6, "\033[1;5D"};
    AnsiKeySequence right_arrow{6, "\033[1;5C"};
    AnsiKeySequence up_arrow{6, "\033[1;5A"};
    AnsiKeySequence down_arrow{6, "\033[1;5B"};
};
CtrlData ctrl;
KeyData keys;
ShiftData shift;

class Key {
public:
    std::vector<unsigned char> sequence;

public:
    Key() {}
    Key(unsigned char c) {
        sequence.push_back(c);
    }
    Key(std::vector<unsigned char>& _sequence) {
        operator=(_sequence);
    }
    void operator=(std::vector<unsigned char>& _sequence) {
        sequence.clear();
        if(_sequence.size() == 0) return;
        if(_sequence[0] != '\033') {
            int utf_result = utf_len(_sequence[0]);
            sequence.reserve(utf_result);
            for(int i = 0; i < utf_result; i++) {
                sequence.push_back(_sequence.at(i));
            }
           return;
        }
        if(_sequence.size() == 1) { sequence.push_back('\033'); return; }
        if(_sequence.at(1) != '[') { sequence.push_back('\033'); return; }
            for(int k = 0; k < 6; k++) {
                if(_sequence[k] == 0) return;
                sequence.push_back(_sequence[k]);
            }
            return;
    }
    bool is_none() {
        return sequence[0] == 0;
    }
    void debug(AnsiKeySequence _sequence) {
        std::cout << "\r\nDebug: ";
        for(int i = 0; i < _sequence.size; i++) {
            std::cout << (int)_sequence.sequence[i] << "; ";
        }
        std::cout << std::flush;
    }
   void debug() {
        std::cout << "\r\nDebug: ";
        for(unsigned char c: sequence) {
            std::cout << (int)c << "; ";
        }
        std::cout << std::flush;
    }
    size_t len() {
        return sequence.size();
    }
    Key& operator=(Key& other) {
        sequence = other.sequence;
        return *this;
    }
    Key& operator=(Key&& other) {
        sequence.swap(other.sequence);
        return *this;
    }
    bool operator==(unsigned char c) {
        if(sequence.size() == 1) return c == sequence[0];
        return false;
    }
    bool operator==(AnsiKeySequence& _sequence) {
        for(int i = 0; i < _sequence.size; i++) {
            if(sequence.size() <= i) { return false; }
            if(_sequence.sequence[i] != sequence[i]) { return false; }
        }
        return true;
    }
    friend class KeyDetection;
};

class KeyDetection {
private:
    std::vector<unsigned char> keys;

private:
    void get_sequence() {
        int id;
        id = keys.size();
        keys.reserve(256);
        for(int j = 0; j < 256; j++)
            keys.push_back('\0');
        int bytes_read = 1;
        keys.clear();
        while(bytes_read > 0) {
            keys.reserve(256);
            for(int j = 0; j < 256; j++)
                keys.push_back('\0');
            bytes_read = read(STDIN_FILENO, keys.data(), 256);
            if(bytes_read < 256) break;
            id += 256;
        }
    }

public:
    void update() {
        get_sequence();
    }
    bool is(unsigned char c) {
        if(keys.size() == 0) return c == 0;
        return c == keys[0];
    }
    bool is(AnsiKeySequence sequence) {
        int j = 0;
        for(int i = 0; j < sequence.size; i++) {
            if(keys.size() <= j) { return false; }
            if((unsigned char)sequence.sequence[i] != keys[j]) { return false; }
            j++;
        }
        return true;
    }
    Key get() {
        return Key(keys);
    }
};

struct CursorStyle {
    bool visible : 1;
    bool blink : 1;
    bool thin : 1;
    CursorStyle() : visible(1), blink(1), thin(1) {}
};

class Cursor {
private:
    CursorStyle current;
    
public:
    bool visible : 1;
    bool blink : 1;
    bool thin : 1;
 
    unsigned int row = 0;
    unsigned int col = 0;

private:
    bool handle_style(bool _current, bool required, const char* switch_on, const char* switch_off) {
        if(_current == required) return false;
        std::cout << "\033[?";
        if(required) std::cout << switch_off;
        else std::cout << switch_on;
        return true;
    }
    void render() {
        if(handle_style(current.visible, visible, "25h", "25l")) current.visible = visible;
        if(handle_style(current.blink, blink, "12h", "25h")) current.blink = blink;
        if(handle_style(current.thin, thin, "12h", "25h")) current.thin = thin;
        if(visible) std::cout << "\033[" << row << ";" << col << "H";
    }

public:
    Cursor() : visible(1), blink(1), thin(1) {}
    template<typename DisplayTemplateArgument>
    friend class RivuletCore;
};

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
