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

private:
    std::string _name;
    int _id;
    std::list <treasure*> _treasures;
    
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










