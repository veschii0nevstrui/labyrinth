#include <vector>
#include <cstdint>
#include <exception>
#include <stdexcept>
#include <random>
#include <cmath>
#include <iostream>

std::mt19937 rnd(std::random_device{}());

using std::cin;
using std::cout;
using std::vector;
using std::endl;

enum cell {
    empty,
    river,
    out,
    arsenal,
    hospital
};

enum direction {
    left = 0,
    up = 1,
    right = 2,
    down = 3
};

struct point {
    int x, y;
    point(int x = 0, int y = 0) : x(x), y(y) {}
    point operator + (point p) {
        return point(x + p.x, y + p.y);
    }
    bool operator == (point p) {
        return x == p.x && y == p.y;
    }
};

point dxy[] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

bool get_bit(__int8_t a, int i) {
    return a & (1 << i);
}

void set_bit(__int8_t &a, int i, bool b) {
    if (b == 0) {
        a &= (~(1 << i));
    } else {
        a |= (1 << i);
    }
}

class board {
public:
    board(int n = 3, int m = 4) {
        n_ = n;
        m_ = m;
        cells_.assign(n_, vector <cell> (m_, empty));
        walls_.assign(m_, vector <__int8_t> (m_, 0));
        generate();
    }
    void generate() {
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
        int p = int(log((long double)n_ * m_) * 2.0);
        for (int i = 0; i + 1 < n_; ++i) {
            for (int j = 0; j + 1 < m_; ++j) {
                int t = rnd() % (n_ * m_);
                if (t > p) {
                    set_bit(walls_[i][j], right, 1);
                    set_bit(walls_[i + dxy[right].x][j + dxy[right].y], left, 1);
                }
                t = rnd() % (n_ * m_);
                if (t > p) {
                    set_bit(walls_[i][j], down, 1);
                    set_bit(walls_[i + dxy[down].x][j + dxy[down].y], up, 1);
                }
            }
        }
    }
    void write() {
        cout << " ";
        for (int i = 0; i < m_; ++i) {
            if (get_bit(walls_[0][i], up)) {
                cout << "_ ";
            } else {
                cout << "  ";
            }
        }
        cout << endl;
        for (int i = 0; i < n_; ++i) {
            for (int j = 0; j < m_; ++j) {
                if (get_bit(walls_[i][j], left)) {
                    cout << "|";
                } else {
                    cout << " ";
                }
                if (get_bit(walls_[i][j], down)) {
                    cout << "_";
                } else {
                    cout << " ";
                }
            }
            if (get_bit(walls_[i][m_ - 1], right)) {
                cout << "|";
            } else {
                cout << " ";
            }
            cout << endl;
        }
    }
/*


    _ _ _ _
   | |

*/
    bool is_wall(point p, direction d) {
        if (!in_board(p)) {
            throw std::invalid_argument("Point is not in board");
        }

        return get_bit(walls_[p.x][p.y], d);
    }
    bool is_wall(point p, point q) {
        if (!in_board(p) || !in_board(q)) {
            throw std::invalid_argument("Point is not in board");
        }

        for (int d = left; d <= down; d += 1) {
            if (p + dxy[d] == q) {
                return is_wall(p, d);
            }
        }

        throw std::invalid_argument("Points are not close.");
    }
    cell in_cell(point p) {
        return cells_[p.x][p.y];
    }
private:
    vector <vector <cell> > cells_;
    vector <vector <__int8_t> > walls_; //bit representation of the walls around the cell
                                        // 0 - left
                                        // 1 - up
                                        // 2 - right
                                        // 0 - down
    bool in_board(point p) {
        return (0 <= p.x && p.x < n_ && 0 <= p.y && p.y < m_);
    }
    int n_, m_;
    bool connected() {
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
    
    // НЕ РАБОТАЕТ СО СТЕНАМИ ИСПРАВЬ НАХУЙ
    void dfs(point v, vector <vector <bool> > &used) {
        used[v.x][v.y] = 1;
        for (int d = left; d <= down; ++d) {
            point nv = v + dxy[d];
            if (in_board(nv) && !used[nv.x][nv.y]) {
                dfs(nv, used);
            }
        }
    }
};
