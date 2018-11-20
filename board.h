#pragma once
#include <vector>
#include <cstdint>
#include <exception>
#include <stdexcept>
#include <random>
#include <cmath>
#include <iostream>
#include <algorithm>

#include "setting.h"
#include "dsu.h"
#include "cells.h"
#include "point.h"
#include "object.h"
#include "hystory.h"

std::mt19937 rnd(std::random_device{}());

using std::cin;
using std::cout;
using std::vector;
using std::string;
using std::endl;

class game;

class board {
public:
    board() {
        _n = 0;
        _m = 0;
    }
    board(setting &s) {
        _n = s.n;
        _m = s.m;
        _setting = s;
        
        generate();
    }
    ~board() {
        for (auto &v : _cells) {
            for (auto &cell : v) {
                delete cell;
            }
        }
    }

    void write() {
        vector <vector <string> > new_map(_n + 1);
        new_map[0].push_back(" ");
        for (int i = 0; i < _m; ++i) {
            if (_cells[0][i]->is_wall(up)) {
                new_map[0].push_back("_ ");
            } else {
                new_map[0].push_back("  ");
            }
        }
        
        for (int i = 0; i < _n; ++i) {
            for (int j = 0; j < _m; ++j) {
                string str;
                if (_cells[i][j]->is_wall(left)) {
                    str = "|";
                } else {
                    str = " ";
                }
                
                std::string wall_down = "_";
                std::string not_wall = " ";
                
                if (_cells[i][j]->type() == "river_flow") {
                    wall_down = "\033[44m_\033[0m";
                    not_wall = "\033[44m \033[0m";
                }
                if (_cells[i][j]->type() == "river_end") {
                    wall_down = "\033[42m_\033[0m";
                    not_wall = "\033[42m \033[0m";
                }
                if (_cells[i][j]->is_human()) {
                    wall_down = "\033[48;5;208m_\033[0m";
                    not_wall = "\033[48;5;208m \033[0m";
                }

                if (_cells[i][j]->is_wall(down)) {
                    str += wall_down;
                } else {
                    str += not_wall;
                }
                new_map[i + 1].push_back(str);
            }
            
            if (_cells[i][_m - 1]->is_wall(right)) {
                new_map[i + 1].push_back("|");
            } else {
                new_map[i + 1].push_back(" ");
            }
        }
        map m(new_map);
        hystory::add(m);
        //m.write();// Расскоментируйте, если хотите, чтобы в процессе показывалась карта
    }

    void write_id() {
        for (int i = 0; i < _n; ++i) {
            for (int j = 0; j < _m; ++j) {
                int id = _cells[i][j]->get_id();
                if (id == -1) {
                    cout << " ";
                } else {
                    cout << id;
                }
            }
            cout << endl;
        }
    }

    friend game;

private:
    vector <vector <cell*> > _cells;
    int _n, _m;
    setting _setting;

    bool in_board(point p) {
        return (0 <= p.x && p.x < _n && 0 <= p.y && p.y < _m);
    }
    
    void generate() {

        _cells.assign(_n, vector <cell*> (_m, nullptr));
        for (int i = 0; i < _n; ++i) {
            for (int j = 0; j < _m; ++j) {
                _cells[i][j] = new empty_cell(point{i, j}, 1 | 2 | 4 | 8);
            }
        }

        generate_tree();
        remove_random_walls();
        generate_rivers();
        add_neighbors();
    }
    
    void generate_tree() {
        dsu d(_n * _m);
        std::vector <std::pair <point, direction> > edges;
        
        for (int i = 0; i + 1 < _n; ++i) {
            for (int j = 0; j + 1 < _m; ++j) {
                edges.push_back({{i, j}, right});
                edges.push_back({{i, j}, down});
            }
        }
        for (int i = 0; i + 1 < _n; ++i) {
            edges.push_back({{i, _m - 1}, down});
        }
        for (int j = 0; j + 1 < _m; ++j) {
            edges.push_back({{_n - 1, j}, right});
        }

        std::random_shuffle(edges.begin(), edges.end(), [](int mod){ return rnd() % mod;});
        // ЗАЧЕКАТЬ, ЧТО ЭТА ШТУКА СЛУЧАЙНА И РАВНОМЕРНА
        
        for (auto edge : edges) {
            point v = edge.first;
            point nv = v + dxy[edge.second];
            if (!d.same(get_point_id(v), get_point_id(nv))) {
                d.my_union(get_point_id(v), get_point_id(nv));
                
                _cells[v.x][v.y]->del_wall(edge.second);
                _cells[nv.x][nv.y]->del_wall(back_dir(edge.second));
            }
        }
    }

    int get_point_id(point p) { //TODO сделать лямбдой
        return p.x * _m + p.y;
    }
    
    void remove_random_walls() {
        int count_edges = 2 * _n * _m - _n - _m;
        int p = (int)(_setting.p_wall * count_edges);
        for (int i = 0; i + 1 < _n; ++i) {
            for (int j = 0; j + 1 < _m; ++j) {
                random_edge({i, j}, right, p, count_edges);
                random_edge({i, j}, down, p, count_edges);
            }
        }
        for (int i = 0; i + 1 < _n; ++i) {
            random_edge({i, _m - 1}, down, p, count_edges);
        }
        for (int j = 0; j + 1 < _m; ++j) {
            random_edge({_n - 1, j}, right, p, count_edges);
        }
    }
    
    void random_edge(point v, direction d, int p, int q) {
        if (rnd() % q < p) {
            point nv = v + dxy[d];
            _cells[v.x][v.y]->del_wall(d);
            _cells[nv.x][nv.y]->del_wall(back_dir(d));
        }
    }
    
    void generate_rivers() {
        int cnt_free = _n * _m;
        int id_river = 0;
        vector <vector <bool> > used(_n, vector <bool> (_m, 0));

        while (id_river < _setting.cnt_source && cnt_free > 0) {
            int num = rnd() % cnt_free;
            point start = 0;
            for (int i = 0; i < _n; ++i) {
                for (int j = 0; j < _m; ++j) {
                    if (!used[i][j]) {
                        if (num == 0) {
                            int len = std::max(2, (int)rnd() % (_setting.mexp_len_river * 2));
                            dfs_river({i, j}, used, len, 1, id_river, cnt_free);
                            --num;
                            break;
                        } else {
                            --num;
                        }
                    }
                }
                if (num == -1) {
                    break;
                }
            }
        }
    }

    void dfs_river( point v, 
                    vector <vector <bool> > &used, 
                    int len, 
                    bool is_start, 
                    int &id_river, 
                    int &cnt_free) {

        used[v.x][v.y] = 1;
        --cnt_free;

        vector <direction> ds = {left, right, up, down};
        std::random_shuffle(ds.begin(), ds.end());

        bool fl = 0;
        if (len > 0) {
            for (auto d : ds) {
                point nv = v + dxy[d];
                if (in_board(nv) && !used[nv.x][nv.y] && !_cells[v.x][v.y]->is_wall(d)) {
                   
                   _cells[v.x][v.y] = new river_flow(_cells[v.x][v.y], d, id_river); 
                    dfs_river(nv, used, len - 1, 0, id_river, cnt_free);
                    
                    fl = 1;
                    break;
                }
            }
        }
        if (!fl) {
            if (!is_start) {
                _cells[v.x][v.y] = new river_end(_cells[v.x][v.y], id_river);
                ++id_river;
            }
            return;
        }
    }
    void add_neighbors() {
        for (int i = 0; i < _n; ++i) {
            for (int j = 0; j < _m; ++j) {
                point v = {i, j};
                for (int d = 0; d < 4; ++d) {
                    point nv = v + dxy[d];
                    cell *ans = nullptr;
                    if (in_board(nv)) {
                        ans = _cells[nv.x][nv.y];
                    }
                    _cells[v.x][v.y]->add_neigh(ans);
                }
            }
        }
    }
    
    
    
};




















