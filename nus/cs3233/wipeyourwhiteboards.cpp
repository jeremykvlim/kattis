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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        long long R, S, Q;
        cin >> R >> S >> Q;

        auto [_, x, y] = extended_gcd(R, -S);
        auto A = [&](auto m) {
            return Q / __gcd(R, -S) * x + m * S / __gcd(R, -S);
        };

        auto B = [&](auto m) {
            return Q / __gcd(R, -S) * -y - m * R / __gcd(R, -S);
        };

        long long l = INT_MIN, r = INT_MAX, m;
        while (l + 1 < r) {
            m = l + (r - l) / 2;

            if (A(m) > 0 && B(m) > 0) l = m;
            else r = m;
        }
        cout << A(l) << " " << B(l) << "\n";
    }
}