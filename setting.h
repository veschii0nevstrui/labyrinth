#pragma once

class setting {
public:
    
    long double p_wall;
    int cnt_source;
    int mexp_len_river;
    int n, m;

    int cnt_human;

    setting(long double p = 1.0, int cnt = 0, int mexp_len = 0, int n = 0, int m = 0, int cnt_human = 0) : 
        n(n),
        m(m),
        p_wall(p), 
        cnt_source(cnt), 
        cnt_human(cnt_human),
        mexp_len_river(mexp_len) {}
private:
};
