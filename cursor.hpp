#pragma once
#include<iostream>


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

