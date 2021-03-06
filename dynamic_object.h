#pragma once
#include <list>
#include <string>
#include "object.h"
#include "class_names.h"

class dynamic_object : public object {
public:
    virtual std::string type() {
        return DYNAMIC_OBJECT;
    }
};

class treasure : public dynamic_object {
public:
    treasure(bool is_true = 1, int id = 0) : 
        _is_true(is_true), 
        _id(id) {}

    std::string type() {
        return TREASURE;
    }
    bool is_true() {
        return _is_true;
    }
    ~treasure() {}
private:
    bool _is_true;
    int _id;
};

class human : public dynamic_object {
public:
    human(std::string name = "noname", int id = 0) : 
        _name(name),
        _id(id) {}

    ~human() {
        for (auto &t : _treasures) {
            delete t;
        }
    }

    std::string type() {
        return HUMAN;
    }
        
    void write() {
        
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
    
    int get_id() {
        return _id;
    }

    std::string get_name() {
        return _name;
    }

    void add_treasure(treasure *tr) {
        _treasures.push_back(tr);
    }

    treasure* get_treasure() {
        if (!_treasures.empty()) {
            return *_treasures.begin();
        } else {
            return nullptr;
        }
    }

    void del_treasure(treasure *tr) {
        for (auto id = _treasures.begin(); id != _treasures.end(); ++id) {
            if (*id == tr) {
                _treasures.erase(id);
                break;
            }
        }
    }

private:
    std::string _name;
    int _id;
    std::list <treasure*> _treasures;
};










