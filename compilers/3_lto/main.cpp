#include "foo.hpp"
#include <iostream>

signed main() {

    std::vector<int> data;
    std::cin >> data;
    std::cout << min(data) << '\n';
    std::cout << data;

    return 0;
}