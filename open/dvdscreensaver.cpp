#include <bits/stdc++.h>
using namespace std;

template <typename T>
array<T, 3> extended_gcd(const T &a, const T &b) {
    if (b == (T) 0) return {a, (T) 1, (T) 0};

    auto div = [&](const T &x, const T &y) {
        if constexpr (!requires(T z) { z.real(); z.imag(); }) return x / y;
        else {
            T numer = x * conj(y);
            auto denom = norm(y);
            auto round_div = [&](auto part) {
                return (part >= 0) ? (part + denom / 2) / denom : (part - denom / 2) / denom;
            };
            return (T) {round_div(numer.real()), round_div(numer.imag())};
        }
    };

    T q = div(a, b), r = a - q * b;
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
