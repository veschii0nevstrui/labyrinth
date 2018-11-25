#pragma once
#include <string>
#include "class_names.h"

using std::cout;
using std::endl;

/*
Говнокодище. Я мог бы определить хороший конструктор объекта,
Но из-за ссылок друг на друга я вынужден копипастить его в двух классах
наследниках. Мне кажется, что пора бросать программирование и начать
заниматься, например, агрономией. Там такой хуйни нет.
*/


class object {
public:
    virtual std::string type() {
        return OBJECT;
    }
    virtual ~object() {}
private:
};

