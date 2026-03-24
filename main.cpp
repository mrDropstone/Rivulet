#include"cube.hpp"
#include"unistd.h"
#include <iomanip>
int main() {
    Cube cube;
    while(cube.render()) {}
        // usleep(10000);
    return 0;
}
