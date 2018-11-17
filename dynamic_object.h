#pragma once
#include <list>
#include <string>
#include "object.h"

class dynamic_object : public object {
public:
    dynamic_object(cell *cell) {
        _cell = cell;
        _cell->add_object(this);
    }
    virtual std::string type() {
        return "dynamic_object";
    }
    virtual point get_coords() {
        return _cell->get_coords();
    }
protected:
    cell *_cell;
private:
};

class treasure : public dynamic_object {
public:
    treasure(cell *cell, bool is_true = 1, int id = 0) : 
        _is_true(is_true), 
        dynamic_object(cell),
        _id(id) {}
    std::string type() {
        return "treasure";
    }
    bool is_true() {
        return _is_true;
    }
private:
    bool _is_true;
    int _id;
};

class human : public dynamic_object {
public:
    human(cell *cell, std::string name = "noname", int id = 0) : 
        dynamic_object(cell),
        _name(name),
        _id(id) {}

    ~human() {
        for (auto &t : _treasures) {
            delete t;
        }
    }

    std::string type() {
        return "human";
    }
        
    void write() {
        _cell->get_coords().write();
        cout << _cell->type() << endl;;
        _cell->write_objects();
        
        cout << "In my bag: ";
        bool fl = 0;
        for (auto obj : _treasures) {
            cout << obj->type() << " ";
            fl = 1;
        }
        if (!fl) {
            cout << "Nothing";
        }
        cout << endl;
    }

    std::string get_cell_type() {
        return _cell->type();
    }
    
    int get_id() {
        return _id;
    }

    std::string get_name() {
        return _name;
    }

    void add_treasure(treasure *tr) {
        _treasures.push_back(tr);
    }

    void move(direction dir) {
        if (!_cell->is_wall(dir)) {
            auto old = swap(dir);
            
            if (_cell->type() == "river_flow") {
                if ((old->type() != "river_flow" && old->type() != "river_end") || 
                    old->get_id() != _cell->get_id()) {
                    
                    swim(2);
                }
            }
        }
    }

    void take_treasure() {
        object* tr = _cell->get_treasure();
        if (tr == nullptr) {
            return;
        }
        _treasures.push_back(tr);
        _cell->del_object(tr);
    }

    void drop_treasure() {
        for (auto it = _treasures.begin(); it != _treasures.end(); ++it) {
            _cell->add_object(*it);
            _treasures.erase(it);
            break;
        }
    }

    bool try_out() {
        while (!_treasures.empty()) {
            auto it = _treasures.begin();
            if ((*it)->is_true()) {
                _treasures.erase(it);
                return 1;
            } else {
                _treasures.erase(it);
            }
        }
    }

private:
    std::string _name;
    int _id;
    std::list <object*> _treasures;
    
    void swim(int cnt) {
        if (cnt == 0) {
            return;
        }
        if (_cell->type() == "river_end") {
            return;
        }
        swap(_cell->get_dir());
        swim(cnt - 1);
    }
    
    cell *swap(direction dir) {
        _cell->del_object(this);
        auto old = _cell;
        _cell = _cell->get_neigh(dir);
        _cell->add_object(this);
        return old;
    }
};










