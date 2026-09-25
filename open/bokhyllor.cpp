#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int a, b, c, s;
    cin >> a >> b >> c >> s;
    int n = s / 3;

    auto floor_div = [](int x, int y) {
        return x >= 0 ? x / y : -(-x + y - 1) / y;
    };

    auto ceil_div = [&](int x, int y) {
        return -floor_div(-x, y);
    };

    int shelves = max(ceil_div(a + 2 * b + 3 * c, s), ceil_div(c, n));
    if (s & 1) shelves = max(shelves, ceil_div(2 * (b + c), s - 1));
    if ((s & 1) != (n & 1)) shelves = max(shelves, ceil_div(2 * b + 4 * c, s + n - 1));
    cout << shelves;
}
