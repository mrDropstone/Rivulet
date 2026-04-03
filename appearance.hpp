#pragma once
#include <stdint.h>
#include <iostream>

namespace Rivulet {
struct Color {
    bool is_default_color = true;
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
    bool operator==(const Color& other) {
        if(other.is_default_color) {
            if(is_default_color) {
                return other.red == red;
            }
            return false;
        }
        return red == other.red && green == other.green && blue == other.blue;
    }
    bool operator!=(const Color& other) {
        return !operator==(other);
    }
};

struct Style {
    bool bold : 1;
    bool dim : 1;
    bool italic : 1;
    bool underline : 1;
    bool blink : 1;
    bool reverse : 1;
    bool invisible : 1;
    bool strike : 1;
    Style() : bold(1), dim(0), italic(0), underline(0), blink(0), reverse(0), invisible(0), strike(0) {}
    bool operator==(Style other) {
        return *((int8_t*)this) == *(int8_t*)(&other);
    }
};

struct Appearance {
    Style style;
    Color bg;
    Color fg;
    bool operator==(const Appearance& other) {
        return *((int64_t*)&bg.is_default_color) == *((int64_t*)&other.bg.is_default_color) && style == other.style;
    }
    bool operator!=(const Appearance& other) {
        return !operator==(other);
    }
};


Appearance applied_appearance;


inline bool handle_appearance(bool current, bool required, int enable_seq) {
    if(current == required) return false;
    std::cout << "\033[";
    if(current) std::cout << 20 + enable_seq;
    else std::cout << enable_seq;
    std::cout << 'm';
    return true;
}

inline void handle_colors(Color& current, Color required, bool is_background) {
    if(current != required) {
        current = required;
        if(current.is_default_color) {
            std::cout << "\033[" << (3 + is_background) << "8;5;" << (int)current.red;
        } else {
            std::cout << "\033[" << (3 + is_background) << "8;2;" << (int)current.red << ';' << (int)current.green << ';' << (int)current.blue;
        }
        std::cout << 'm';
    }
}

void apply_appearance(Appearance& appearance) {
    if(applied_appearance != appearance) {
        if(handle_appearance(applied_appearance.style.dim, appearance.style.dim, 2))  { applied_appearance.style.dim = appearance.style.dim; if(!applied_appearance.style.dim) applied_appearance.style.bold = false;}
        if(handle_appearance(applied_appearance.style.bold, appearance.style.bold, 2))  { applied_appearance.style.bold = appearance.style.bold; if(!applied_appearance.style.bold) applied_appearance.style.dim = false;}
        if(handle_appearance(applied_appearance.style.italic, appearance.style.italic, 3))   applied_appearance.style.italic = appearance.style.italic;
        if(handle_appearance(applied_appearance.style.underline, appearance.style.underline, 4))   applied_appearance.style.underline = appearance.style.underline;
        if(handle_appearance(applied_appearance.style.blink, appearance.style.blink, 5))   applied_appearance.style.blink = appearance.style.blink;
        if(handle_appearance(applied_appearance.style.reverse, appearance.style.reverse, 7))   applied_appearance.style.reverse = appearance.style.reverse;
        if(handle_appearance(applied_appearance.style.invisible, appearance.style.invisible, 8))   applied_appearance.style.invisible = appearance.style.invisible;
        if(handle_appearance(applied_appearance.style.strike, appearance.style.strike, 9))   applied_appearance.style.strike = appearance.style.strike;
    }
    handle_colors(applied_appearance.fg, appearance.fg, false);
    handle_colors(applied_appearance.bg, appearance.bg, true);
}
}
