#include <iostream>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <map>
#include "game.h"

using std::cin;
using std::cout;
using std::endl;
using std::map;

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

map <char, direction> mp;
game *g = nullptr;

void handler(int signal) {
    if (g != nullptr) {
        set_canonical_mod();
        delete g;
    }
    exit(0);
}

int main() {
    signal(SIGINT, handler);
    mp['w'] = up;
    mp['a'] = left;
    mp['s'] = down;
    mp['d'] = right;
    int n, m;
    cin >> n >> m;
    long double p;
    int cnt_r, len;
    int cnt_h;
    cin >> p;
    cin >> cnt_r >> len;
    //cin >> cnt_h;
    cnt_h = 1;

    setting s(p, cnt_r, len, n, m, cnt_h);
    g = new game(s);
    hystory::add(' ');
    g->write();
    set_non_canonical_mod();    
    
    while (true) {
        char c;
        cin >> c;
        if (c == 'p') {
            hystory::add(c);
            cout << "end" << endl;
            break;
        }
        if (c == 'e') {
            hystory::add(c);
            g->take(0);
        }
        if (c == 'q') {
            hystory::add(c);
            g->drop(0);
        }
        if (c == 'f') {
            hystory::add(c);
            if (g->try_out(0)) {
                cout << "You win!\nCongratulation!!" << endl;
                break;
            }
        }
        if (mp.find(c) != mp.end()) {
            hystory::add(c);
            g->move(0, mp[c]);
        }
    }
    getchar();
    cout << "-------------------------" << endl;
    cout << "-------------------------" << endl;
    cout << "Hystory:" << endl; 
    hystory::write();
    set_canonical_mod();
    delete g;
}
