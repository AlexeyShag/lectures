#include <bits/stdc++.h>


std::mt19937 rnd(time(0));
signed main() {

    size_t len = 1e6;
    while (len != 0) {
        size_t t = rnd() % 1000 + 1000;
        if (t > len) t = len;
        std::cout << t << '\n';
        for (size_t i = 0; i < t; ++i) {
            std::cout << rnd() % 10 << ' ';
        }
        std::cout << '\n';
        len -= t;
    }

    return 0;
}