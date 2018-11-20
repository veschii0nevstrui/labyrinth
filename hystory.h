#pragma once
#include <vector>
#include <string>
#include <iostream>

using std::vector;
using std::string;
using std::cout;
using std::endl;

class map {
public:
    map(vector <vector <string> > map) {
        _map = map;
    }
    void write() {
        for (int i = 0; i < _map.size(); ++i) {
            for (int j = 0; j < _map[i].size(); ++j) {
                cout << _map[i][j];
            }
            cout << endl;
        }
        cout << endl;
    }
private:
    vector <vector <string> > _map;
};

class hystory {
public:
    static void write(int i) {
        hystory::_maps[i].write();
    }
    static void write() {
        for (int i = 0; i < _maps.size(); ++i) {
            cout << _symbs[i] << endl;
            hystory::_maps[i].write();
        }
    }
    static void add(map &m) {
        hystory::_maps.push_back(m);
    }
    static void add(char ch) {
        hystory::_symbs.push_back(ch);
    }
private:
    static vector <map> _maps;
    static vector <char> _symbs;
};

vector <map> hystory::_maps; //Мне не нравится, что она у всех на виду, надо как-то спрятать
vector <char> hystory::_symbs;


