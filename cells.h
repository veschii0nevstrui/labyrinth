#pragma once
#include <string>
#include <vector>
#include <list>
#include <stdexcept>

#include "point.h"
#include "object.h"
#include "dynamic_object.h"
#include "class_names.h"

using std::string;
using std::cout;
using std::endl;

class cell {
public:
    cell(__int8_t mask_walls = 0) : 
        _mask_walls(mask_walls)
        {}

    cell(cell *c) :
            _mask_walls(c->_mask_walls)
            {
        delete c;
    }

    virtual string type() {
        return CELL;
    }
    
    ~cell() {
        for (auto &obj : _objects) {
            delete obj;
        }
    }

    virtual void write() {
        cout << "Type cell: " << type() << endl;
        cout << "In cell: ";
        write_objects();
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
        return -1;
    }

    virtual direction get_dir() {
        throw std::invalid_argument("I'm not river flow!");
    }

    virtual cell* action(human *h, int old_id) {
        return this;
    }

    virtual void add_neigh(cell *neigh) {
        _neighbors.push_back(neigh);
    }
    
    virtual cell* get_neigh(direction dir) {
        return _neighbors[dir];
    }

    virtual void add_object(object *obj) {
        _objects.push_back(obj);
    }

    virtual object* get_object(const string &type) {
        for (auto obj : _objects) {
            if (obj->type() == type) {
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

    virtual bool is_object(const string &type) {
        for (auto h : _objects) {
            if (h->type() == type) {
                return 1;
            }
        }
        return 0;
    }

    virtual void write_objects() {
        bool fl = 0;
        for (auto obj : _objects) {
            if (obj->type() != HUMAN) {
                fl = 1;
                cout << obj->type() << " ";
            }
        }
        if (!fl) {
            cout << "Nothing";
        }
        cout << endl;
    }

protected:
    __int8_t _mask_walls;
    std::vector <cell*> _neighbors; // соседи TODO придумать норм название
    std::list <object*> _objects;
private:
};

class empty_cell : public cell {
public:
    empty_cell(     __int8_t mask_walls) : 
                    cell(mask_walls) 
                    {}

    empty_cell(cell* c) : cell(c) {}
    string type() {
        return EMPTY_CELL;
    }
private:
};

class river_flow : public cell {
public:
    river_flow( direction dir, 
                int id, 
                __int8_t mask_walls
                ):     
                    _dir(dir), 
                    _id(id), 
                    cell(mask_walls) 
                    {}
    river_flow(cell *c, direction dir, int id) : 
        cell(c), 
        _dir(dir),
        _id(id)
        {}

    string type() {
        return RIVER_FLOW;
    }
    
    int get_id() {
        return _id;
    }

    direction get_dir() {
        return _dir;
    }

    cell* action(human *h, int old_id) {
        auto ptr = _objects.begin();
        for (; ptr != _objects.end(); ++ptr) {
            if (*ptr == h) {
                break;
            }
        }
        if (ptr == _objects.end()) {
            throw std::invalid_argument("There are not this human!");
        }

        if (old_id == _id) {
            cout << "You are riverwalker" << endl;
            return this;
        }

        auto ans = dfs(this, 2);//Захардкоженная константа, фу таким быть!
        
        cout << "You found yourself in river flow and was carry" << endl;

        _objects.erase(ptr);
        ans->add_object(h);

        return ans;
    }

private:
    direction _dir;
    int _id;

    cell* dfs(cell *v, int cnt) {
        if (cnt == 0 || v->type() == RIVER_END) {
            return v;
        }
        return dfs(v->get_neigh(v->get_dir()), cnt - 1); //МБ добавить для потока get_next?
    }
};

class river_end : public cell {
public:
    river_end(  int id, 
                __int8_t mask_walls) : 
                    _id(id), 
                    cell(mask_walls) 
                    {}

    river_end(cell *c, int id) : cell(c), _id(id) {}

    cell *action(human *h, int old_id) {
        //Дальше идёт копипаста, нехорошо. И всё ради того,
        //чтобы сказать, что пидорас риверволкер.
        auto ptr = _objects.begin();
        for (; ptr != _objects.end(); ++ptr) {
            if (*ptr == h) {
                break;
            }
        }
        if (ptr == _objects.end()) {
            throw std::invalid_argument("There are not this human!");
        }

        if (old_id == _id) {
            cout << "You are riverwalker" << endl;
        }
        return this;
    }

    string type() {
        return RIVER_END;
    }
    int get_id() {
        return _id;
    }
private:
    int _id;
};
