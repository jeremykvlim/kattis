#include <bits/stdc++.h>
using namespace std;

template <typename T>
array<T, 3> extended_gcd(const T &a, const T &b) {
    if (b == (T) 0) return {a, (T) 1, (T) 0};

    T q = a / b, r = a - q * b;
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