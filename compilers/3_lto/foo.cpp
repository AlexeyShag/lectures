#include "foo.hpp"
#include <vector>
#include <iostream>
#include <cassert>

std::istream& operator>>(std::istream& _in, std::vector<int>& _values) {
    size_t len;
    _in >> len;
    _values.resize(len);
    for (size_t i = 0; i < len; ++i) {
        _in >> _values[i];
    }
    return _in;
}

std::ostream& operator<<(std::ostream& _out, const std::vector<int>& _values) {
    _out << _values.size() << '\n';
    for (size_t i = 0; i < _values.size() - 1; ++i) {
        _out << _values[i] << ' ';
    }
    _out << _values.back() << '\n';
    return _out;
}

int min(const std::vector<int>& _values) {
    assert(!_values.empty());
    int ans = _values[0];
    for (auto& val: _values) {
        ans = std::min<int>(val, ans);
    }
    return ans;
}

