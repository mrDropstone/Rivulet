#pragma once 
#include<iostream>

void print_space(int times) {
    while (times > 0) {
        std::cout << ' ';
        times--;
    }
}
