#include <bits/stdc++.h>
using namespace std;

template <typename T, typename U, typename V>
T mul(U x, V y, T mod) {
    return (unsigned __int128) x * y % mod;
}

template <typename T, typename U>
pair<T, T> fib(U n, T mod = 1) {
    if (!n) return {0, 1 % mod};

    auto add = [&](T x, T y) {
        return x >= mod - y ? x - (mod - y) : x + y;
    };

    auto sub = [&](T x, T y) {
        return x >= y ? x - y : mod - (y - x);
    };

    auto [f1, f2] = fib(n >> 1, mod);
    T fib1 = mul(f1, sub(add(f2, f2), f1), mod), fib2 = add(mul(f1, f1, mod), mul(f2, f2, mod));
    if (n & 1) return {fib2, add(fib1, fib2)};
    return {fib1, fib2};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int p;
    cin >> p;

    while (p--) {
        long long k, y;
        cin >> k >> y;
        cout << k << " " << fib(y, (int) 1e9).first << "\n";
    }
}
