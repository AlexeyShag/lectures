#include <vector>
#include <iostream>

#define sequence_t std::vector<int>
#define YES std::cout << "YES\n"
#define NO std::cout << "NO\n"

bool is_sorted(const sequence_t& arr) {
  for (int i = 0; i < arr.size() - 1; i++) {
    if (arr[i] > arr[i + 1]) {
      return false;
    }
  }
  return true;
}

signed main() {
  size_t len;
  std::cin >> len;

  sequence_t arr(len);
  for (auto& a: arr) {
    std::cin >> a;
  }

  if (is_sorted(arr)) {
    YES;
  } else {
    NO;
  }
  return 0;
}