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

    long long b;
    int d;
    string a;
    cin >> b >> d >> a;

    int l = 0, r = 1e5, m;
    while (l + 1 < r) {
        m = l + (r - l) / 2;

        auto p10 = 1LL, c = 0LL;
        for (int i = 0; i < m; i++) {
            p10 = (p10 * 10) % b;
            c = (c * 10 + d) % b;
        }

        auto x = linear_congruence_solution(p10, (b - c) % b, b);
        if (x == -1) {
            r = m;
            continue;
        }

        if (!x && !d) x = b / __gcd(p10, b);
        auto s = to_string(x);
        if (!x) s.clear();
        s += string(m, '0' + d);

        if (s.size() == a.size() ? s <= a : s.size() < a.size()) l = m;
        else r = m;
    }

    cout << l;
}
