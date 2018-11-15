#pragma once
#include <string>
#include "object.h"
#include "cells.h"

class static_object : public object {
public:
    static_object(cell *cell) {
        _cell = cell;
        _cell->add_object(this);
    }
    virtual std::string type() {
        return "static_object";
    }
private:
    cell *_cell;
};

class arsenal : public static_object {
public:
    arsenal(cell *cell) : static_object(cell) {}
    std::string type() {
        return "arsenal";
    }
private:
};

class hospital : public static_object {
public:
    hospital(cell *cell) : static_object(cell) {}
    std::string type() {
        return "hospital";
    }
private:
};

class out : public static_object {
public:
    out(cell *cell) : static_object(cell) {}
    std::string type() {
        return "out";
    }
private:
};
