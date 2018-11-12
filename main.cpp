#include <iostream>
#include "board.h"

using namespace std;

int main() {
    setting s(1);
    while (true) {
        int n, m;
        cin >> n >> m;
        long double d;
        cin >> d;
        s.p = d;
        if (n == 0) {
            break;
        }
        board b(s, n, m);
        b.write();
        cout << endl;
    }
}
