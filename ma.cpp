#include"rivulet.hpp"
using namespace Rivulet;

int main() {
    Grid grid;
    grid.render();
    grid.display
        .nth(5, 3)
        .overwrite("hel");
    grid.display
        .nth(7, 9)
        .overwrite("hel");
    grid.render();
    while (1) {}
    return 0;
}
