#include <iostream>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

using std::cin;
using std::cout;
using std::endl;

struct termios old_attributes;
struct termios new_attributes;

void set_non_canonical_mod()
{
    tcgetattr(0, &old_attributes);
    memcpy(&new_attributes, &old_attributes, sizeof(struct termios));
    new_attributes.c_lflag &= ~ECHO;
    new_attributes.c_lflag &= ~ICANON;
    new_attributes.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &new_attributes);
}

void set_canonical_mod()
{
    tcsetattr(0, TCSANOW, &old_attributes);
}

direction ch[300];

int main() {
    ch['w'] = up;
    ch['a'] = left;
    ch['s'] = down;
    ch['d'] = right;
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
        cnt_h = 1;

        setting s(p, cnt_r, len, n, m, cnt_h);
        game g(s);
        g.write();
        set_non_canonical_mod();    
        while (true) {
            char c;
            cin >> c;
            if (c == 'e') {
                cout << "end" << endl;
                break;
            }
            g.move(0, ch[c]);
            g.write();
        }
        set_canonical_mod();
    }
}
