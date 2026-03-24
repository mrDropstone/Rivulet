#include"style.hpp"
#pragma once

inline int utf_len(unsigned char character) {
    if ((character & 0b10000000) == 0)       return 1;
    if ((character & 0b11100000) == 0b11000000) return 2;
    if ((character & 0b11110000) == 0b11100000) return 3;
    if ((character & 0b11111000) == 0b11110000) return 4;
    throw std::runtime_error("Invalid.");
}


class u8_char { // utf-8 character
private:
    char _data;

public:
    Style style;
    u8_char() {}
    u8_char(unsigned char c) : _data(c) {}
    int set(const char* chars, Style _style) {
        style = _style;
        _data = chars[0];
        return 1;
    }
    void render() {
        apply_style(style);
        std::cout << _data;
    }
    u8_char& operator=(unsigned char c) {
        _data = c;
        return *this;
    }
    bool operator==(u8_char& other) {
        return style == other.style && _data == other._data; 
    }
    bool operator!=(u8_char& other) {
        return !operator==(other);
    }
};



class u32_char { // utf-8 character
private:
    std::array<unsigned char, 4> _data;

public:
    Style style;
    u32_char() {}
    u32_char(unsigned char c) {
        _data[0] = c;
        _data[1] = 0;
    }
    int set(const char* chars, Style _style) {
        int utf_result = utf_len(chars[0]);
        style = _style;
        int i = 0;
        while(i < utf_result) {
            _data[i] = chars[i];
            i++;
        }
        if(i != 4)_data[i] = '\0';
        return utf_result;
    }
    void render() {
        apply_style(style);
        for(char c: _data) {
            if(c == 0) break;
            std::cout << c;
        }
    }
    u32_char& operator=(unsigned char c) {
        _data[0] = c; _data[1] = 0;
           return *this;
    }
    bool operator==(u32_char& other) {
        return style == other.style && (int32_t)(_data[0]) == (int32_t)(other._data[0]); 
    }
    bool operator!=(u32_char& other) {
        return !operator==(other);
    }
};



template<typename CharacterType = u32_char>
class line {
private:
    std::vector<CharacterType> _line_data;

public:
    int overwrite(const char* chars, unsigned int col, Style& style) { // It returns how much the col changed
        if(col > 3500) return 0;
        if(_line_data.size() < col) {
            int times = col - _line_data.size() + 1;
            _line_data.reserve(times);
            for(int i = 0; i < times; i++) {
                _line_data.emplace_back(' ');
            }
        }
        int i = 0;
        while(col < _line_data.size()) {
            if(chars[i] == '\0') return 0;
            i += _line_data.at(col).set(&chars[i], style);
            col++;
        }
        while(chars[i] != '\0') {
            _line_data.emplace_back('\0');
            i += _line_data.at(col).set(&chars[i], style);
            col++;
        }
        return 0;
    }
    void render(unsigned int rows_limit) {
        for(int i = 0; i < _line_data.size() && i < rows_limit; i++) {
            _line_data[i].render();
        }
    }
    void swap(const CharacterType& other) {
        other._line_data.swap(_line_data);
    }
    size_t size() {
        return _line_data.size();
    }
    size_t len() {
        return _line_data.size();
    }
    auto begin() {
        return _line_data.begin();
    }
    void reserve(size_t bytes) {
        _line_data.reserve(bytes);
    }
    void push_back(const CharacterType& c) {
        _line_data.push_back(c);
    }
    CharacterType& operator[](int index) {
        return _line_data.at(index);
    }
    void fill(int amount, const CharacterType& value) {
        while(amount >= 0) {
            _line_data.push_back(value);
            amount--;
        }
    }
    void refill_with(const CharacterType& value) {
        size_t currentSize = _line_data.size();
        _line_data.clear();
        fill(currentSize, value);
    }
    template<typename LineDataContainer>
    friend class Line;
};


template<typename LineDataContainer=line<u32_char>>
class Line {
private:
    LineDataContainer _data;
    LineDataContainer _actual_screen;
    bool _is_modified = true;

public:
    Line() {}

    void _overwrite(const char* chars, unsigned int col, Style& style) {
        _data.overwrite(chars, col, style);
        _is_modified = true;
    }

    inline size_t len() {
        return _data.len();
    }
    void clear() {
        _data.refill_with(' ');
        _is_modified = true;
    }
    void fill(unsigned int amount) {
        _data.fill(amount, ' ');
        _actual_screen.fill(amount, ' ');
        _is_modified = true;
    }
    void screen_cleared() {
        _actual_screen.refill(' ');
        _is_modified = true;
    }


    void render_from(unsigned int col, unsigned int target) {
        auto data_it = _data.begin();
        data_it += col - 1;
        for(int i = col; i < target && i < _data.size(); i++) {
            (*data_it).render();
            data_it++;
        }
    }
    void render(unsigned int cols, int row) {
        if(!_is_modified)
            return;
        if(_data.size() > _actual_screen.size() && cols > _actual_screen.size())
            _actual_screen.fill(cols - _actual_screen.size(), ' ');
        _is_modified = false;
        int data_it = 0;
        int i = 1;
        int last_render_col = 1;
        bool rendered = true;
        while(i <= _actual_screen.size()) {
            if(i >= cols || i == _data.size())
                return;
            if(_data[data_it] != _actual_screen[data_it]) {
                if(last_render_col != i) {
                    if(i - last_render_col > 8) {
                        if(rendered) {
                            std::cout << "\033[" << row << ";" << i - last_render_col << "H";
                            rendered = false;
                        } else {
                            std::cout << "\033["<< i - last_render_col << "C";
                        }
                    } else {
                        render_from(last_render_col, i);
                    }
                    data_it = i - 1;
                    last_render_col = i;
                }
                _actual_screen[data_it] = _data._line_data[data_it];
                _data[data_it].render();
                last_render_col++;
            }
            data_it++;
            i++;
        }
    }
    void debug() {
        for(u32_char cv : _actual_screen._line_data) {
            cv.render();
        }
        std::cout << std::flush;
    }
    template<typename LineContainer>
    friend class Display;
};


