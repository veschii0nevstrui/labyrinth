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

std::mt19937 rnd(std::random_device{}());

using std::cin;
using std::cout;
using std::vector;
using std::endl;

class board {
public:
    board(setting &s, int n = 3, int m = 4) {
        n_ = n;
        m_ = m;
        setting_ = s;
        
        generate();
    }
    void write() {
        cout << " ";
        for (int i = 0; i < m_; ++i) {
            if (cells_[0][i]->is_wall(up)) {
                cout << "_ ";
            } else {
                cout << "  ";
            }
        }
        cout << endl;
        
        for (int i = 0; i < n_; ++i) {
            for (int j = 0; j < m_; ++j) {
                if (cells_[i][j]->is_wall(left)) {
                    cout << "|";
                } else {
                    cout << " ";
                }
                
                std::string wall_down = "_";
                std::string not_wall = " ";
                if (cells_[i][j]->type() == "river_flow") {
                    wall_down = "\033[44m_\033[0m";
                    not_wall = "\033[44m \033[0m";
                }
                if (cells_[i][j]->type() == "river_end") {
                    wall_down = "\033[42m_\033[0m";
                    not_wall = "\033[42m \033[0m";
                }

                if (cells_[i][j]->is_wall(down)) {
                    cout << wall_down;
                } else {
                    cout << not_wall;
                }
            }
            
            if (cells_[i][m_ - 1]->is_wall(right)) {
                cout << "|";
            } else {
                cout << " ";
            }
            cout << endl;
        }
    }

    void write_id() {
        for (int i = 0; i < n_; ++i) {
            for (int j = 0; j < m_; ++j) {
                int id = cells_[i][j]->get_id();
                if (id == -1) {
                    cout << " ";
                } else {
                    cout << id;
                }
            }
            cout << endl;
        }
    }

private:
    vector <vector <cell*> > cells_;
    int n_, m_;
    setting setting_;

    bool in_board(point p) {
        return (0 <= p.x && p.x < n_ && 0 <= p.y && p.y < m_);
    }
    
    void generate() {

        cells_.assign(n_, vector <cell*> (m_, nullptr));
        for (auto &v : cells_) {
            for (auto &cell : v) {
                cell = new empty_cell(1 | 2 | 4 | 8);
            }
        }

        generate_tree();
        remove_random_walls();
        generate_rivers();
    }
    
    void generate_tree() {
        dsu d(n_ * m_);
        std::vector <std::pair <point, direction> > edges;
        
        for (int i = 0; i + 1 < n_; ++i) {
            for (int j = 0; j + 1 < m_; ++j) {
                edges.push_back({{i, j}, right});
                edges.push_back({{i, j}, down});
            }
        }
        for (int i = 0; i + 1 < n_; ++i) {
            edges.push_back({{i, m_ - 1}, down});
        }
        for (int j = 0; j + 1 < m_; ++j) {
            edges.push_back({{n_ - 1, j}, right});
        }

        std::random_shuffle(edges.begin(), edges.end(), [](int mod){ return rnd() % mod;});
        // ЗАЧЕКАТЬ, ЧТО ЭТА ШТУКА СЛУЧАЙНА И РАВНОМЕРНА
        
        for (auto edge : edges) {
            point v = edge.first;
            point nv = v + dxy[edge.second];
            if (!d.same(get_point_id(v), get_point_id(nv))) {
                d.my_union(get_point_id(v), get_point_id(nv));
                
                cells_[v.x][v.y]->del_wall(edge.second);
                cells_[nv.x][nv.y]->del_wall(back_dir(edge.second));
            }
        }
    }

    int get_point_id(point p) { //TODO сделать лямбдой
        return p.x * m_ + p.y;
    }
    
    void remove_random_walls() {
        int count_edges = 2 * n_ * m_ - n_ - m_;
        int p = (int)(setting_.p_wall * count_edges);
        for (int i = 0; i + 1 < n_; ++i) {
            for (int j = 0; j + 1 < m_; ++j) {
                random_edge({i, j}, right, p, count_edges);
                random_edge({i, j}, down, p, count_edges);
            }
        }
        for (int i = 0; i + 1 < n_; ++i) {
            random_edge({i, m_ - 1}, down, p, count_edges);
        }
        for (int j = 0; j + 1 < m_; ++j) {
            random_edge({n_ - 1, j}, right, p, count_edges);
        }
    }
    
    void random_edge(point v, direction d, int p, int q) {
        if (rnd() % q < p) {
            point nv = v + dxy[d];
            cells_[v.x][v.y]->del_wall(d);
            cells_[nv.x][nv.y]->del_wall(back_dir(d));
        }
    }
    
    void generate_rivers() {
        int cnt_free = n_ * m_;
        int id_river = 0;
        vector <vector <bool> > used(n_, vector <bool> (m_, 0));

        while (id_river < setting_.cnt_source && cnt_free > 0) {
            int num = rnd() % cnt_free;
            point start = 0;
            for (int i = 0; i < n_; ++i) {
                for (int j = 0; j < m_; ++j) {
                    if (!used[i][j]) {
                        if (num == 0) {
                            int len = std::max(2, (int)rnd() % (setting_.mexp_len_river * 2));
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
                if (in_board(nv) && !used[nv.x][nv.y] && !cells_[v.x][v.y]->is_wall(d)) {
                    
                    __int8_t mask = cells_[v.x][v.y]->get_mask();
                    free(cells_[v.x][v.y]);
                    cells_[v.x][v.y] = new river_flow(d, id_river, mask);

                    dfs_river(nv, used, len - 1, 0, id_river, cnt_free);
                    
                    fl = 1;
                    break;
                }
            }
        }
        if (!fl) {
            if (!is_start) {

                /*
                Копипаста. Исправляется написанием нормального конструктора копирования
                TODO
                */
                __int8_t mask = cells_[v.x][v.y]->get_mask();
                free(cells_[v.x][v.y]);
                cells_[v.x][v.y] = new river_end(id_river, mask);
                
                ++id_river;
            }
            return;
        }
    }
    /*
    bool connected() { //may be useless
        vector <vector <bool> > used(n_, vector <bool> (m_, 0));
        dfs(point(0, 0), used);
        for (int i = 0; i < n_; ++i) {
            for (int j = 0; j < m_; ++j) {
                if (!used[i][j]) {
                    return 0;
                }
            }
        }
        return 1;
    }
    
    void dfs(point v, vector <vector <bool> > &used) { //may be useless
        used[v.x][v.y] = 1;
        for (int d = 0; d < 4; ++d) {
            point nv = v + dxy[d];
            if (in_board(nv) && !used[nv.x][nv.y] && !is_wall(v, d)) {
                dfs(nv, used);
            }
        }
    }
    
    void generate_border() { // may be useless in our generation method
        cells_.assign(n_, vector <cell> (m_, empty));
        walls_.assign(n_, vector <__int8_t> (m_, 0));
        
        for (int i = 0; i < m_; ++i) {
            set_bit(walls_[0][i],     up,   1);
            set_bit(walls_[n_ - 1][i], down, 1);
        }

        for (int i = 0; i < n_; ++i) {
            set_bit(walls_[i][0],     left,  1);
            set_bit(walls_[i][m_ - 1], right, 1);
        }
    }*/

    

    
    
    
    
    
};




















