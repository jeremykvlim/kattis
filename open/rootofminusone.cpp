#include <bits/stdc++.h>
using namespace std;

template <typename T, typename U, typename V>
T mul(U x, V y, T mod) {
    return (unsigned __int128) x * y % mod;
}

template <typename T, typename U>
T pow(T base, U exponent, T mod) {
    T value = 1;
    while (exponent) {
        if (exponent & 1) value = mul(base, value, mod);
        base = mul(base, base, mod);
        exponent >>= 1;
    }
    return value;
}

template <typename T>
T tonelli_shanks(T n, T p) {
    if (p == 2) return n;
    if (p % 4 == 3) return pow(n, (p + 1) / 4, p);

    T q = p - 1;
    int s = 0;
    while (!(q & 1)) {
        q >>= 1;
        s++;
    }

    auto legendre = [&](T a, T p) -> int {
        if (!a) return 0;
        return pow(a, (p - 1) / 2, p) != p - 1 ? 1 : -1;
    };

    T z = 2;
    while (legendre(z, p) != -1) z++;

    int m = s;
    T c = pow(z, q, p), t = pow(n, q, p), r = pow(n, (q + 1) / 2, p);
    while (t != 1) {
        auto temp = t;

        int i = 0;
        while (temp != 1 && i < m) {
            temp = mul(temp, temp, p);
            i++;
        }

        T b = pow(c, 1LL << (m - i - 1), p);
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
