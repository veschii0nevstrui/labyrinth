#pragma once
#include "board.h"
#include "setting.h"
#include "static_object.h"
#include "dynamic_object.h"
#include "class_names.h"

class game {
public:
    game(setting &s) : 
        _b(s), 
        _n(s.n),
        _m(s.m),
        _s(s)
    {
        add_humans();
        add_treasures();
        add_statics();
    }

    void write() {
        for (int i = 0; i < _humans.size(); ++i) {
            _cells[i]->write();
            _humans[i]->write();
        }
        _b.write();
        cout << endl;
    }

    void move(int num, direction dir) {
        int old_id = _cells[num]->get_id();

        if (_cells[num]->is_wall(dir)) {
            cout << "No, you are touch wall" << endl;
            write();
            return;
        }
        cout << "Ok, you move" << endl;

        _cells[num]->del_object(_humans[num]);
        _cells[num] = _cells[num]->get_neigh(dir);
        _cells[num]->add_object(_humans[num]);
        _cells[num] = _cells[num]->action(_humans[num], old_id);

        write();
    }

    void take(int num) {
        treasure *t = static_cast<treasure*>(_cells[num]->get_object(TREASURE));
        if (t == nullptr) {
            cout << "Sorry, there are not any treasure(" << endl;
            write();
            return;
        }

        _humans[num]->add_treasure(t);
        _cells[num]->del_object(t);

        write();
    }

    void drop(int num) {
        auto t = _humans[num]->get_treasure();
        if (t == nullptr) {
            cout << "Sorry, you have not treasure(" << endl;
            write();
            return;
        }

        _humans[num]->del_treasure(t);
        _cells[num]->add_object(t);

        write();
    }
    
    bool try_out(int num) {
        if (!_cells[num]->is_object(OUT)) {
            cout << "Sorry, there are not out" << endl;
            write();
            return 0;
        }

        auto t = _humans[num]->get_treasure();
        bool ans = 0;
        if (t == nullptr) {
            cout << "Sorry, you have not treasure((" << endl;
        } else {
            if (t->is_true()) {
                ans = 1;
            } else {
                cout << "Sorry, you treasure is lie(((" << endl;
            }
            _humans[num]->del_treasure(t);
            delete t;
        }
        write();
        return ans;
    }

private:
    board _b;
    int _n, _m;
    setting _s;
    
    vector <human*> _humans;
    vector <cell*> _cells;

    void add_humans() {
        for (int cnt = 0; cnt < _s.cnt_human; ++cnt) {
            int i = rnd(_n);
            int j = rnd(_m);
            
            auto h = new human("Martin", cnt);
            _humans.push_back(h);
            _cells.push_back(_b[i][j]);
            _b[i][j]->add_object(h);
        }
    }
    void add_treasures() {
        for (int q = 0; q < 2; ++q) {
            int i = rnd(_n);
            int j = rnd(_m);

            auto t = new treasure(q, q);
            _b[i][j]->add_object(t);
        }
    }
    void add_statics() {
        int i = rnd(_n);
        int j = rnd(_m);
        out *o = new out();
        _b[i][j]->add_object(o);
    }
};










