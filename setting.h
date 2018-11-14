#pragma once

class setting {
public:
    
    long double p_wall;
    int cnt_source;
    int mexp_len_river;

    setting(long double p = 1.0, int cnt = 0, int mexp_len = 0) : p_wall(p), cnt_source(cnt), mexp_len_river(mexp_len) {}
private:
};
