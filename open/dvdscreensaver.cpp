#include <bits/stdc++.h>
using namespace std;

template <typename T>
pair<T, T> bezout(T a, T b) {
    if (!a) return {0, 1};
    auto [x, y] = bezout(b % a, a);
    return {y - (b / a) * x, x};
}

template <typename T>
T linear_congruence_solution(T a, T b, T n) {
    auto g = __gcd(a, n);
    if (b % g) return -1;

    a /= g;
    b /= g;
    n /= g;
    auto [x, y] = bezout(a, n);
    x = (x + n) % n;
    return (b * x) % n;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        long long ws, hs, wl, hl, xl, yl;
        cin >> ws >> hs >> wl >> hl >> xl >> yl;

        ws -= wl;
        hs -= hl;
        auto a = ws % hs, b = ((hs - yl) % hs - ((ws - xl) % ws) % hs + hs) % hs, x = linear_congruence_solution(a, b, hs);
        cout << ((x == -1) ? "Johnny will die waiting\n" : to_string(x * ws + (ws - xl) % ws) + "\n");
    }
}
