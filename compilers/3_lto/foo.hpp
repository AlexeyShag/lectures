#include <vector>
#include <iostream>
#include <cassert>

std::istream& operator>>(std::istream&, std::vector<int>&);

std::ostream& operator<<(std::ostream& _out, const std::vector<int>& _values);

int min(const std::vector<int>&);

