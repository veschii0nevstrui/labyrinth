#pragma once
#include <string>
#include <vector>
#include <list>
#include <stdexcept>
#include "point.h"
#include "object.h"

using std::string;
using std::cout;
using std::endl;

class cell {
public:
    cell(point coords, __int8_t mask_walls = 0) : 
        _mask_walls(mask_walls),
        _coords(coords)
        {}

    cell(cell *c) :
            _coords(c->_coords),
            _mask_walls(c->_mask_walls)
            {
        delete c;
    }

    virtual string type() {
        return "cell";
    }
    
    ~cell() {
        for (auto &obj : _objects) {
            delete obj;
        }
    }
        
    virtual void add_wall(int dir) {
        set_bit(_mask_walls, dir, 1);
    }

    virtual void del_wall(int dir) {
        set_bit(_mask_walls, dir, 0);
    }

    virtual bool is_wall(int dir) {
        return get_bit(_mask_walls, dir);
    }

    virtual int get_id() {
        throw std::invalid_argument("I'm not river!");
    }

    virtual direction get_dir() {
        throw std::invalid_argument("I-m not river flow!");
    }

    virtual void add_neigh(cell *neigh) {
        _neighbors.push_back(neigh);
    }
    
    virtual cell* get_neigh(direction dir) {
        return _neighbors[dir];
    }

    virtual point get_coords() {
        return _coords;
    }

    virtual void add_object(object *obj) {
        _objects.push_back(obj);
    }

    virtual object* get_treasure() {
        for (auto obj : _objects) {
            if (obj->type() == "treasure") {
                return obj;
            }
        }
        return nullptr;
    }

    virtual void del_object(object *obj) {
        for (auto it = _objects.begin(); it != _objects.end(); ++it) {
            if (*it == obj) {
                _objects.erase(it);
                break;
            }
        }
    }

    virtual void write_objects() {
        bool fl = 0;
        cout << "There are in cell: ";
        for (auto obj : _objects) {
            if (obj->type() != "human") {
                fl = 1;
                cout << obj->type() << " ";
            }
        }
        if (!fl) {
            cout << "Nothing";
        }
        cout << endl;
    }

    virtual void write() {
        for (auto o : _objects) {
            cout << o->type() << ", ";
        }
        cout << endl;
        for (int i = 0; i < 4; ++i) {
            cout << "    " << i << " ";
            if (_neighbors[i] != nullptr) {
                cout << _neighbors[i]->type() << endl;
            } else {
                cout << "out border" << endl;
            }
        }
    }

    virtual bool is_human() {
        for (auto h : _objects) {
            if (h->type() == "human") {
                return 1;
            }
        }
        return 0;
    }

    virtual bool is_out() {
        for (auto h : _objects) {
            if (h->type() == "out") {
                return 1;
            }
        }
        return 0;
    }

private:
    __int8_t _mask_walls;
    std::vector <cell*> _neighbors; // соседи TODO придумать норм название
    std::list <object*> _objects;
    point _coords;
};

class empty_cell : public cell {
public:
    empty_cell( point p, 
                __int8_t mask_walls = 0) : 
                    cell(p, mask_walls) 
                    {}

    empty_cell(cell* c) : cell(c) {}
    string type() {
        return "empty_cell";
    }
private:
};

class river_flow : public cell {
public:
    river_flow( point p,
                direction dir, 
                int id, 
                __int8_t mask_walls = 0
                ):     
                    _dir(dir), 
                    _id(id), 
                    cell(p, mask_walls) 
                    {}
    river_flow(cell *c, direction dir, int id) : 
        cell(c), 
        _dir(dir),
        _id(id)
        {}

    string type() {
        return "river_flow";
    }
    int get_id() {
        return _id;
    }
    direction get_dir() {
        return _dir;
    }
private:
    direction _dir;
    int _id;
};

class river_end : public cell {
public:
    river_end(  point p,
                int id, 
                __int8_t mask_walls = 0) : 
                    _id(id), 
                    cell(p, mask_walls) 
                    {}

    river_end(cell *c, int id) : cell(c), _id(id) {}

    string type() {
        return "river_end";
    }
    int get_id() {
        return _id;
    }
private:
    int _id;
};
