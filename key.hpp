#pragma once
#include<cstddef>
#include<iostream>
#include<vector>


namespace Rivulet {
class Key {
private:
    std::vector<char> _data;

public:
    Key() {}
    Key(char c) {_data.push_back(c);}
    Key(const std::vector<char>& sequence) {
        _data = sequence;
    }
    Key(const char *chars) {
        for (int i = 0; chars[i] != '\0'; i++) {
            _data.push_back(chars[i]);
        }
    }
    Key(const char *chars, int len) {
        for (int i = 0; chars[i] && i < len; i++) {
            _data.push_back(chars[i]);
        }
    }
    Key& operator=(const Key &other) {
        _data = other._data;
        return *this;
    }
    Key& operator=(Key &&other) {
        _data.swap(other._data);
        return *this;
    }
    bool operator==(char c) {
        if(_data.size() == 1)
            return c == _data[0];
        return false;
    }
    bool operator==(const Key& other) {
        return other._data == _data;
    }
    std::string to_string() {
        std::string str;
        for (unsigned char c : _data) {
            str += c;
        }
        return str;
    }
    int get_byte(int index) {
        return _data.at(index);
    }
    bool is_none() {
        if (_data.size() == 0)
            return true;
        return _data[0] == 0;
    }
    void debug() {
        std::cout << "\r\nDebug: ";
        for(unsigned char c: _data) {
            std::cout << (int)c << "; ";
        }
        std::cout << std::flush;
    }
    size_t len() {
        return _data.size();
    }
    friend class KeyDetection;
};



const struct {
    Key left_arrow{"\033[1;2D"};
    Key right_arrow{"\033[1;2C"};
    Key up_arrow{"\033[1;2A"};
    Key  down_arrow{"\033[1;2B"};
    Key tab{"\033[\0;\17"};
} shift;

const struct {
    Key right_arrow{"\x1B\x5B\x43"};
    Key left_arrow {"\x1B\x5B\x44"};
    Key up_arrow{"\x1B\x5B\x41"};
    Key down_arrow {"\x1B\x5B\x42"};
    Key tab = '\10';
    Key esc = '\x1B';
    Key home{"\033\133\61\176"};
} keys;


const struct {
    Key left_arrow{"\033[1;5D"};
    Key right_arrow{"\033[1;5C"};
    Key up_arrow{"\033[1;5A"};
    Key down_arrow{"\033[1;5B"};
    Key home{"\033\133\61\73\110"};
     
} ctrl;

const struct {
    Key home{"\033\133\61\73\63\110"};
} alt;
}
