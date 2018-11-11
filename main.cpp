#include <iostream>
#include "board.h"

using namespace std;

int main() {
    board b(3, 4);
    while (true) {
        b.write();
        int c;
        cin >> c;
        b.generate();
        if (!c) {
            break;
        }
    }
}
