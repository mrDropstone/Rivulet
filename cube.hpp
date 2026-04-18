#pragma once
#include"rivulet.hpp"
#include<cmath>
using namespace Rivulet;
struct Actual {
    int x;
    int y;
    int size;
    Actual(int _x, int _y) : x(_x), y(_y) {}
    Actual() {}
};
int Centre_x = 100;
int Centre_y = 40;
const double PI = std::acos(-1.0);

double to_radians(double _degrees) {
    return _degrees * PI / 180.0;
}

double to_degrees(double _radians) {
    return _radians * 180.0 / PI;
}

double Cos(double _degrees) {
    return std::cos(to_radians(_degrees));
}

double Sin(double _degrees) {
    return std::sin(to_radians(_degrees));
}
const char depth_chars[] = {
    '.',    // 0: Furthest - tiny detail
    ',',    // 1: Very far
    '-',    // 2: Far
    '~',    // 3: Mid-far
    ':',    // 4: Medium distance
    ';',    // 5: Mid-close
    '=',    // 6: Getting closer
    '+',    // 7: Close
    '#',    // 8: Very close
    '@'     // 9: Closest - full detail
};

template<typename T>
struct Virtual {
    T x;
    T y;
    T z;
    Actual actual(double direction_x, double direction_y, double direction_z) const {
        Virtual<double> copy;
        copy.x = x; copy.y = y; copy.z = z;
        

        // std::cout << "\r\ncopy.x=" << copy.x << ", copy.y=" << copy.y << ", copy.z=" << copy.z << std::flush;
        copy.y = y * Cos(direction_x) - z * Sin(direction_x);
        copy.z = y * Sin(direction_x) + z * Cos(direction_x);
        // std::cout << "\r\ncopy.x=" << copy.x << ", copy.y=" << copy.y << ", copy.z=" << copy.z << std::flush;
        double copy_x = copy.x;
        double copy_z = copy.z;
        copy.x = copy_x * Sin(direction_y) + copy_z * Cos(direction_y);
        copy.z = copy_x * Cos(direction_y) - copy_z * Sin(direction_y);
        // std::cout << "\r\ncopy.x=" << copy.x << ", copy.y=" << copy.y << ", copy.z=" << copy.z << std::flush;
        copy_x = copy.x;
        copy.x = copy.x * Cos(direction_z) - copy.y * Sin(direction_z);
        copy.y = copy_x * Sin(direction_z) + copy.y * Cos(direction_z);
        // std::cout << "\r\ncopy.x=" << copy.x << ", copy.y=" << copy.y << ", copy.z=" << copy.z << std::flush;
        copy.z += 230;
        Actual result;
        result.x = copy.x;
        result.y = copy.y;
        result.size = copy.z;
        return result;
    }
    Virtual(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
    Virtual() {}
};

class Cube {
private:
    Grid zincul;
    int last_x = 50;
    int last_y = 50;
    std::vector<Virtual<double>> elements;
    int dx, dy, dz = 0;
    bool update = false;
public:
    void draw_line(Actual first, Actual second) {
        int bias = second.x - first.x;
        double coefficent = 0;
        if(bias != 0) {
            coefficent = (second.y - first.y) / bias; 
        }
        int inc;
        if(bias > 0) {
            inc = 1;
        } else {inc = -1;}
        int x = first.x;
        double y = first.y;
        std::cout << "Second.x=" << second.x << "; second.y=" << second.y << " first.x=" << first.x << "; first.y=" << first.y << std::flush;
        for(int i = 0; i < bias; i++) {
            
            zincul.display.nth(x + Centre_x, y + Centre_y).overwrite("@");
            x += inc;
            y += coefficent;
        }
    }
    char distance(int size) {
        return '@';
        size -= 170;
        int l = size / 5 % 10;
        return depth_chars[l];
    }
    void update_screen() {
        Centre_x = zincul.display.size.cols / 2;
        Centre_y = zincul.display.size.rows / 2;
        zincul.display.clear();
        Actual man;
        char loh[2] = {0, 0};
        for(int i = 0; i < elements.size(); i++) {
            man = elements[i].actual(dx, dy, dz);
            loh[0] = distance(man.size);
           
            if(man.x > -Centre_x && man.y > -Centre_y)
                zincul.display
                .nth(man.y + Centre_y, man.x + Centre_x)
                .overwrite((const char*)&loh[0]);
        }
    }
    bool render() {
        Key pressed = zincul.key.get();
        if (pressed == keys.up_arrow) {update_screen();dx = (dx + 5) % 360;zincul.render(); }
        if (pressed == keys.down_arrow) {update_screen();dx = dx - 5; if(dx < 0) dx = 360;zincul.render();}
        if (pressed == keys.right_arrow) {update_screen();dy = dy - 5; if(dy < 0) dy = 360;zincul.render();}
        if (pressed == keys.left_arrow) {update_screen(); dy = (dy + 5) % 360;zincul.render();}
        if (pressed == 'h') {zincul.display.size.update(); update_screen();zincul.render();}
        if (pressed == keys.esc) return false;
        return true;
    }
    Cube() {
        int size = zincul.display.size.cols / 7;
        int begin = - size / 2;
        for(int i = 0; i < size; i++) {
            elements.emplace_back(i + begin, begin, begin);
            elements.emplace_back(begin, i + begin, begin);
            elements.emplace_back(begin, begin, i + begin);
            elements.emplace_back(size + begin, begin, i + begin);
            elements.emplace_back(begin, size + begin, i + begin);
            elements.emplace_back(size + begin, i + begin, begin);
            elements.emplace_back(begin, i + begin, size + begin);
            elements.emplace_back(size - i + begin, size + begin, size + begin);
            elements.emplace_back(size + begin, size - i + begin, size + begin);
            elements.emplace_back(size + begin, size + begin, size - i + begin);
            elements.emplace_back(size + begin, begin, size - i + begin);
            elements.emplace_back(begin, size + begin, size - i + begin);
            elements.emplace_back(size - i + begin, size + begin, begin);
            elements.emplace_back(size - i + begin, begin, size + begin);

        }
        update_screen();
        zincul.render();

        /*
        for(int i = 0; i < size; i++) 
        elements.emplace_back(0, 0 + i, 0);

        for(int i = 0; i < size; i++) 
        elements.emplace_back(0 + i, 0 , 0);
        
        for(int i = 0; i < size; i++) 
        elements.emplace_back(0, 0 , 0 + i);
        
        for(int i = 0; i < size; i++) 
        elements.emplace_back(size, 0 , 0 + i);
        for(int i = 0; i < size; i++) 
        elements.emplace_back(size, 0 + i, 0);
        for(int i = 0; i < size; i++) 
        elements.emplace_back(i, size, 0);
        for(int i = 0; i < size; i++) 
        elements.emplace_back(0, size, i);
        for(int i = 0; i < size; i++)
        elements.emplace_back(i, 0, size);
        */
    }
};
