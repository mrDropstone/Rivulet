#pragma once


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
    Color bg;
    Color fg;
    Style() : bold(1), dim(0), italic(0), underline(0), blink(0), reverse(0), invisible(0), strike(0) {}
    bool operator==(const Style& other) {
        return *(&bg.is_default_color - 1) == *(&other.bg.is_default_color - 1) && 
            *((int64_t*)&bg.is_default_color) == *((int64_t*)&other.bg.is_default_color);
            //return bold == other.bold && italic == other.italic && blink == other.blink && strike == other.strike && reverse == other.reverse;
    }
    bool operator!=(const Style& other) {
        return !operator==(other);
    }
};


Style applied_style;


inline bool handle_style(bool current, bool required, int enable_seq) {
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

void apply_style(Style& style) {
    if(applied_style != style) {
        if(handle_style(applied_style.dim, style.dim, 2))  { applied_style.dim = style.dim; if(!applied_style.dim) applied_style.bold = false;}
        if(handle_style(applied_style.bold, style.bold, 2))  { applied_style.bold = style.bold; if(!applied_style.bold) applied_style.dim = false;}
        if(handle_style(applied_style.italic, style.italic, 3))   applied_style.italic = style.italic;
        if(handle_style(applied_style.underline, style.underline, 4))   applied_style.underline = style.underline;
        if(handle_style(applied_style.blink, style.blink, 5))   applied_style.blink = style.blink;
        if(handle_style(applied_style.reverse, style.reverse, 7))   applied_style.reverse = style.reverse;
        if(handle_style(applied_style.invisible, style.invisible, 8))   applied_style.invisible = style.invisible;
        if(handle_style(applied_style.strike, style.strike, 9))   applied_style.strike = style.strike;
    }
    handle_colors(applied_style.fg, style.fg, false);
    handle_colors(applied_style.bg, style.bg, true);
}


