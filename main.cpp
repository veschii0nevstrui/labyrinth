#include <iostream>
#include "game.h"

using namespace std;

int main() {
    while (true) {
        int n, m;
        cin >> n >> m;
        if (n == 0) {
            break;
        }
        long double p;
        int cnt_r, len;
        int cnt_h;
        cin >> p;
        cin >> cnt_r >> len;
        cin >> cnt_h;

        setting s(p, cnt_r, len, n, m, cnt_h);
        game g(s);
        g.write();
        cout << "----------------------------------------" << endl;
        for (int i = 0; i < cnt_h; ++i) {
            g.move(i, up);
        }
        g.write();
        cout << "!!!----------------------------------!!!" << endl;
    }
}
