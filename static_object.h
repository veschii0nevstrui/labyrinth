#pragma once
#include <string>
#include "object.h"
#include "class_names.h"

class static_object : public object {
public:
    virtual std::string type() {
        return STATIC_OBJECT;
    }
private:
};

class arsenal : public static_object {
public:
    std::string type() {
        return ARSENAL;
    }
private:
};

class hospital : public static_object {
public:
    std::string type() {
        return HOSPITAL;
    }
private:
};

class out : public static_object {
public:
    std::string type() {
        return OUT;
    }
private:
};
