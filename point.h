#pragma once
#include <iostream>
#include <random>

std::mt19937 _rnd(std::random_device{}());

int rnd(int a) {
    return _rnd() % a;
}

bool rnd(int p, int q) {
    return rnd(q) < p;
}

enum direction {
    left = 0,
    up = 1,
    right = 2,
    down = 3
};

int back_dir(int d) {
    return (d + 2) % 4;
}

struct point {
    int x, y;
    point(int x = 0, int y = 0) : x(x), y(y) {}
    point operator + (point p) {
        return point(x + p.x, y + p.y);
    }
    point operator == (point p) {
        return x == p.x && y == p.y;
    }
    void write() {
        std::cout << x << " " << y << std::endl;
    }
};

point dxy[] = {{0, -1}, {-1, 0}, {0, 1}, {1, 0}};

bool get_bit(__int8_t a, int i) {
    return a & (1 << i);
}

void set_bit(__int8_t &a, int i, bool b) {
    if (b == 0) {
        a &= (~(1 << i));
    } else {
        a |= (1 << i);
    }
}
