#pragma once
#include "board.h"
#include "setting.h"
#include "static_object.h"
#include "dynamic_object.h"

class game {
public:
    game() {
        _n = 0;
        _m = 0;
        _b = board();
        _s = setting();
    }
    game(setting &s) : _b(s) {
        _n = s.n;
        _m = s.m;
        _s = s;
        
        add_humans();
        add_treasures();
        add_statics();
    }

    ~game() {}

    void write() {
        //_b.write(); Uncomment this string if you want to see a board.
        //cout << endl;
        for (auto h : _humans) {
            h->write();
        }
    }

    void move(int num, direction dir) {
        _humans[num]->move(dir);
        write();
    }

    void take(int num) {
        _humans[num]->take_treasure();
        write();
    }

    void drop(int num) {
        _humans[num]->drop_treasure();
        write();
    }
    
    bool try_out(int num) {
        bool ans = _humans[num]->try_out();
        write();
        if (ans) {
            _b.write();
        }
        return ans;
    }

private:
    board _b;
    int _n, _m;
    setting _s;
    vector <human*> _humans;

    void add_humans() {
        for (int cnt = 0; cnt < _s.cnt_human; ++cnt) {
            int i = rnd() % _n;
            int j = rnd() % _m;
            _humans.push_back(new human(_b._cells[i][j], "Martin", cnt));
        }
    }
    void add_treasures() {
        for (int q = 0; q < 2; ++q) {
            int i = rnd() % _n;
            int j = rnd() % _m;
            treasure *t = new treasure(_b._cells[i][j], q, q);
        }
    }
    void add_statics() {
        int i = rnd() % _n;
        int j = rnd() % _m;
        //arsenal *a = new arsenal(_b._cells[i][j]);
        
        i = rnd() % _n;
        j = rnd() % _m;
        //hospital *h = new hospital(_b._cells[i][j]);
        
        //Dont need in first release

        i = rnd() % _n;
        j = rnd() % _m;
        out *o = new out(_b._cells[i][j]);
    }
};










