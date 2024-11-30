#include <bits/stdc++.h>
using namespace std;

template <typename T>
T mul(T x, T y, T mod) {
    long long px = x, py = y, pmod = mod;
    auto product = px * py - pmod * (long long) (1.L / pmod * px * py);
    return product + pmod * (product < 0) - pmod * (product >= pmod);
}

template <typename T>
T pow(T base, T exponent, T mod) {
    T value = 1;
    while (exponent) {
        if (exponent & 1) value = mul(value, base, mod);
        base = mul(base, base, mod);
        exponent >>= 1;
    }
    return value;
}

long long tonelli_shanks(long long n, long long p) {
    if (p == 2) return n;
    if (p % 4 == 3) return pow(n, (p + 1) / 4, p);

    auto q = p - 1;
    int s = 0;
    while (!(q & 1)) {
        q >>= 1;
        s++;
    }

    auto legendre = [&](long long a, long long p) -> int {
        if (!a) return 0;
        return pow(a, (p - 1) / 2, p) != p - 1 ? 1 : -1;
    };

    auto z = 2LL;
    while (legendre(z, p) != -1) z++;

    int m = s;
    auto c = pow(z, q, p), t = pow(n, q, p), r = pow(n, (q + 1) / 2, p);
    while (t != 1) {
        auto temp = t;

        int i = 0;
        while (temp != 1 && i < m) {
            temp = mul(temp, temp, p);
            i++;
        }

        auto b = pow(c, 1LL << (m - i - 1), p);
        m = i;
        c = mul(b, b, p);
        t = mul(t, mul(b, b, p), p);
        r = mul(r, b, p);
    }
    return r;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long p;
    cin >> p;

    auto r = tonelli_shanks(p - 1, p);
    cout << min(r, p - r);
}
