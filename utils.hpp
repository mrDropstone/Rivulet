#pragma once
#include<stdexcept>

inline int utf_len(unsigned char character) {
    if ((character & 0b10000000) == 0)       return 1;
    if ((character & 0b11100000) == 0b11000000) return 2;
    if ((character & 0b11110000) == 0b11100000) return 3;
    if ((character & 0b11111000) == 0b11110000) return 4;
    throw std::runtime_error("Invalid.");
}

