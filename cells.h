#pragma once
#include <string>
#include <vector>
#include <list>
#include "point.h"
#include "object.h"

using std::string;
using std::cout;
using std::endl;

class cell {
public:
    cell(int x = 0, int y = 0, __int8_t mask_walls = 0) : 
        _mask_walls(mask_walls),
        _x(x),
        _y(y) {}
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
    virtual __int8_t get_mask() { // TODO написать нормальный конструктор копирования,
                          // который не будет менять маску.
        return _mask_walls;
    }
    virtual int get_id() {
        return -1;
    }
    virtual void add_neigh(cell *neigh) {
        _neighbors.push_back(neigh);
    }
    virtual cell *get_neigh(direction dir) {
        return _neighbors[dir];
    }

    virtual point get_coords() {
        return point(_x, _y);
    }

    virtual void add_object(object *obj) {
        _objects.push_back(obj);
    }
    virtual void del_object(object *obj) {
        for (auto it = _objects.begin(); it != _objects.end(); ++it) {
            if (*it == obj) {
                _objects.erase(it);
                break;
            }
        }
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

    virtual int sz() {
        return _neighbors.size();
    }

    virtual bool is_human() {
        for (auto h : _objects) {
            if (h->type() == "human") {
                return 1;
            }
        }
        return 0;
    }

private:
    __int8_t _mask_walls;
    std::vector <cell*> _neighbors; // соседи TODO придумать норм название
    std::list <object*> _objects;
    int _x;
    int _y;
};

class empty_cell : public cell {
public:
    empty_cell(int x, int y, __int8_t mask_walls = 0) : cell(x, y, mask_walls) {}
    string type() {
        return "empty_cell";
    }
private:
};

class river_flow : public cell {
public:
    river_flow( int x, 
                int y, 
                direction dir, 
                int id, 
                __int8_t mask_walls = 0
                ):     
                    _dir(dir), 
                    _id(id), 
                    cell(x, y, mask_walls) 
                    {}
    string type() {
        return "river_flow";
    }
    int get_id() {
        return _id;
    }
private:
    direction _dir;
    int _id;
};

class river_end : public cell {
public:
    river_end(int x, int y, int id, __int8_t mask_walls = 0) : _id(id), cell(x, y, mask_walls) {}
    string type() {
        return "river_end";
    }
    int get_id() {
        return _id;
    }
private:
    int _id;
};