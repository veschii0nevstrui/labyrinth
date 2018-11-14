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
        s.p_wall = d;
        int cnt, len;
        cin >> cnt >> len;
        s.cnt_source = cnt;
        s.mexp_len_river = len;
        if (n == 0) {
            break;
        }
        board b(s, n, m);
        b.write();
        cout << endl;
        b.write_id();
        cout << endl;
    }
}
