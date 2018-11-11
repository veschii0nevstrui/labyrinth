#include <iostream>
#include "board.h"

using namespace std;

int main() {
    while (true) {
        int n, m;
        cin >> n >> m;
        if (n == 0) {
            break;
        }
        board b(n, m);
        b.write();
        cout << endl;
    }
}
