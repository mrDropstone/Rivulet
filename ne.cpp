#include<iostream>
#include"rivulet.hpp"
#include <unistd.h>

int main() {
    Rivulet rivulet;
    for (int i = 0; i < 300; i++) {
        Key key = rivulet.key.get();
        key.debug();
        std::cout << "is none=" << key.is_none() << " ";
        if (key == keys.home) {
            std::cout << "COWWWWWW";
        }
        std::cout << key.len() << std::flush;
        usleep(100000);
    }
    return 0;
}
