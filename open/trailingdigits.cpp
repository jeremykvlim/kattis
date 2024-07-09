#include <bits/stdc++.h>
using namespace std;

template <typename T>
pair<T, T> bezout(T a, T b) {
    if (!a) return {0, 1};
    auto [x, y] = bezout(b % a, a);
    return {y - (b / a) * x, x};
}

long long congruence(long long base, long long b, long long mod) {
    auto g = __gcd(base, mod);
    if (b % g) return -1;

    b /= g;
    base /= g;
    mod /= g;
    auto [x, y] = bezout(base, mod);
    x %= mod;
    if (x < 0) x += mod;
    return b * x % mod;
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

        auto base = 1LL, B = 0LL;
        for (int i = 0; i < m; i++) {
            base = base * 10 % b;
            B = (B * 10 + d) % b;
        }

        auto x = congruence(base, (b - B) % b, b);
        if (x == -1) {
            r = m;
            continue;
        }

        if (!x && !d) x = b / __gcd(base, b);
        auto s = to_string(x);
        if (!x) s.clear();
        s += string(m, '0' + d);

        if (s.size() == a.size() ? s <= a : s.size() < a.size()) l = m;
        else r = m;
    }

    cout << l;
}
