#pragma once
#include <vector>

class dsu {
public:
    dsu(int n = 0) {
        size_.assign(n, 1);
        parent_.resize(n);
        for (int i = 0; i < n; ++i) {
            parent_[i] = i;
        }
    }

    void my_union(int u, int v) {
        u = get(u);
        v = get(v);
        if (u == v) {
            return;
        }
        if (size_[u] > size_[v]) {
            std::swap(u, v);
        }
        parent_[u] = v;
        size_[v] += size_[u];
    }

    bool same(int u, int v) {
        return get(u) == get(v);
    }
    
private:
    std::vector <int> size_;
    std::vector <int> parent_;
    
    int get(int v) {
        if (parent_[v] == v) {
            return v;
        }
        parent_[v] = get(parent_[v]);
        return parent_[v];
    }
};
