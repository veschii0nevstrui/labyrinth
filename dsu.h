#pragma once
#include <vector>

class dsu {
public:
    dsu(int n = 0) {
        _size.assign(n, 1);
        _parent.resize(n);
        for (int i = 0; i < n; ++i) {
            _parent[i] = i;
        }
    }

    void my_union(int u, int v) {
        u = get(u);
        v = get(v);
        if (u == v) {
            return;
        }
        if (_size[u] > _size[v]) {
            std::swap(u, v);
        }
        _parent[u] = v;
        _size[v] += _size[u];
    }

    bool same(int u, int v) {
        return get(u) == get(v);
    }
    
private:
    std::vector <int> _size;
    std::vector <int> _parent;
    
    int get(int v) {
        if (_parent[v] == v) {
            return v;
        }
        _parent[v] = get(_parent[v]);
        return _parent[v];
    }
};
