#pragma once
#include<stdint.h>
#include"display.hpp"
#include"key_detect.hpp"
#include"cursor.hpp"
#include"configuration.hpp"

namespace Rivulet {
template<typename DisplayTemplateArgument>
class GridCore {
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

using Grid = GridCore<Line<line<u32_char>>>;
}
