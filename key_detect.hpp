#pragma once
#include"key.hpp"
#include<unistd.h>
#include <vector>

class KeyDetection {
private:
    std::vector<char> _key_data;

private:
    std::vector<char> get_sequence() {
        std::vector<char> key_data;
        int index = 0;
        int bytes_read = 1;
        int total_bytes_read = 0;
        while (bytes_read > 0) {
            key_data.resize(256);
            for (int i = 0; i < 256; i++)
                key_data.push_back(0);
            bytes_read = read(STDIN_FILENO, key_data.data() , 256);
            total_bytes_read += bytes_read;
            if(bytes_read < 256) break;
            index += 256;
        }
        key_data.resize(total_bytes_read);
        return key_data;
    }
    std::vector<char> _get_sequence() {
        std::vector<char> key_data;
        int id;
        id = key_data.size();
        key_data.reserve(256);
        for(int j = 0; j < 256; j++)
            key_data.push_back('\0');
        int bytes_read = 1;
        key_data.clear();
        while(bytes_read > 0) {
            key_data.reserve(256);
            for(int j = 0; j < 256; j++)
                key_data.push_back('\0');
            bytes_read = read(STDIN_FILENO, key_data.data(), 256);
            if(bytes_read < 256) break;
            id += 256;
        }
        if (key_data.size() != 0)
            std::cout << "WOW" << std::flush;
        return key_data;
    }

public:
    Key get() {
        return Key(get_sequence());
    }
};

