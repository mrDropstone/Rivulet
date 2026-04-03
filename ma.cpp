#include"rivulet.hpp"
using namespace Rivulet;

int main() {
    Grid grid;
    grid.render();
    grid.display
        .nth(5, 3)
        .overwrite("hell55345343434343");
    grid.render();
    while (1) {}
    return 0;
}
