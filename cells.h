#pragma once
#include <string>
#include "point.h"
//#include "human.h" TODO

using std::string;

class cell {
public:
    cell(__int8_t mask_walls = 0) : mask_walls_(mask_walls) {}
    virtual string type() {
        return "cell";
    }
    virtual void action() = 0;
    
    virtual void add_wall(int dir) {
        set_bit(mask_walls_, dir, 1);
    }

    virtual void del_wall(int dir) {
        set_bit(mask_walls_, dir, 0);
    }

    virtual bool is_wall(int dir) {
        return get_bit(mask_walls_, dir);
    }
    virtual __int8_t get_mask() { // TODO написать нормальный конструктор копирования,
                          // который не будет менять маску.
        return mask_walls_;
    }
    virtual int get_id() {
        return -1;
    }
private:
    __int8_t mask_walls_;
};

class empty_cell : public cell {
public:
    empty_cell(__int8_t mask_walls = 0) : cell(mask_walls) {}
    string type() {
        return "empty_cell";
    }
    void action() {
        return;
    }
private:
};

class river_flow : public cell {
public:
    river_flow( direction dir, 
                int id, 
                __int8_t mask_walls = 0
                ):     
                    dir(dir), 
                    id(id), 
                    cell(mask_walls) 
                    {}
    string type() {
        return "river_flow";
    }
    void action() { //TODO
        return;
    }
    int get_id() {
        return id;
    }
private:
    direction dir;
    int id;
};

class river_end : public cell {
public:
    river_end(int id, __int8_t mask_walls = 0) : id(id), cell(mask_walls) {}
    string type() {
        return "river_end";
    }
    void action() {
        return;
    }
    int get_id() {
        return id;
    }
private:
    int id;
};
