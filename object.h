#pragma once
#include <string>
#include <iostream>

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
        return "object";
    }
    virtual int get_id() { //TODO придумать, как обойтись без этих двух функций в родителе
        return -1;
    }
    virtual bool is_true() {
        return 0;
    }
    virtual ~object() {}
private:
};

