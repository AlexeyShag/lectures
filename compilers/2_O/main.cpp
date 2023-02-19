int max(int a) {
    return a;
}

template<typename... types_t>
int max(int a, types_t... b) {
    int ans = max(b...);
    return (a < ans ? ans : a);
}

signed main() {
    int t = max(1, 2, 3, 4, 5, 6, 7, 8, 9);
    return 0;
}