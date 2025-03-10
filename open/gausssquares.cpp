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

pair<long long, long long> cornacchia(long long d, long long m) {
    auto a = tonelli_shanks(m - d % m, m);

    auto r0 = m, r1 = a;
    long long sqrt_m = sqrtl(m);
    while (r1 > sqrt_m) {
        auto r2 = r0 % r1;
        r0 = r1;
        r1 = r2;
    }

    auto x = r1;
    if (!d) return {x, 0};

    long long y = sqrtl((m - d * x * x) / d);
    return {x, y};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long p;
    cin >> p;

    auto [x, y] = cornacchia(1, p);
    cout << x << " " << y;
}
