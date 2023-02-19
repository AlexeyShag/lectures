#include <iostream>
#include <algorithm>
#include <chrono>
#include <stdlib.h>

double test(size_t _size) {
    std::vector<int> data(_size);
    for (auto& a: data) {
        std::cin >> a;
    }

    using clock = std::chrono::high_resolution_clock;
    auto start = clock::now();
    std::sort(data.begin(), data.end(), [](auto a, auto b){
        return a > b;
    });
    auto end = clock::now();

    for (auto& a: data) {
        std::cout << a << ' ';
    }

    return (end - start).count();
}

int main(int argc, char** argv) {
    size_t size = 0;
    double sum = 0;
    size_t count = 0;

    while(std::cin >> size) {
        sum +=  test(size);
        ++count;
    }

    std::cerr << sum / count << " ns" << '\n';

    return 0;
}