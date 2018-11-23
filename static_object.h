#pragma once
#include <string>
#include "object.h"
#include "cells.h"

class static_object : public object {
public:
    virtual std::string type() {
        return "static_object";
    }
private:
};

class arsenal : public static_object {
public:
    std::string type() {
        return "arsenal";
    }
private:
};

class hospital : public static_object {
public:
    std::string type() {
        return "hospital";
    }
private:
};

class out : public static_object {
public:
    std::string type() {
        return "out";
    }
private:
};
