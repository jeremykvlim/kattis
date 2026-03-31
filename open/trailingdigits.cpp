#include <bits/stdc++.h>
using namespace std;

template <typename T>
array<T, 3> extended_gcd(const T &a, const T &b) {
    if (b == (T) 0) return {a, (T) 1, (T) 0};

    auto divmod = [&](const T &x, const T &y) -> pair<T, T> {
        if constexpr (requires(T z) { z.real(); z.imag(); }) {
            auto div = [&](const T &x, const T &y) -> T {
                T numer = x * conj(y);
                auto denom = norm(y);
                auto round_div = [&](auto part) {
                    return (part >= 0) ? (part + denom / 2) / denom : (part - denom / 2) / denom;
                };
                return (T) {round_div(numer.real()), round_div(numer.imag())};
            };
            T q = div(x, y), r = x - q * y;
            return {q, r};
        } else {
            T q = x / y, r = x - q * y;
            return {q, r};
        }
    };

    auto [q, r] = divmod(a, b);
    auto [g, s, t] = extended_gcd(b, r);
    return {g, t, s - t * q};
}

template <typename T>
T linear_congruence_solution(T a, T b, T n) {
    T g = __gcd(a, n);
    if (b % g) return -1;

    n /= g;
    auto [_, x, y] = extended_gcd(a / g, n);
    x = (x + n) % n;
    return (b / g * x) % n;
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
