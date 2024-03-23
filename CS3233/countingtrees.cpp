#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO1 = 3, MODULO2 = 11, MODULO3 = 101, MODULO4 = 3000301;

pair<long long, long long> bezout(long long a, long long b) {
    if (!a) return {0, 1};
    auto [x, y] = bezout(b % a, a);
    return {y - (b / a) * x, x};
}

long long crt(long long a, long long m, long long b, long long n) {
    auto g = __gcd(m, n);
    if ((b - a) % g) return -1;

    if (n > m) {
        swap(a, b);
        swap(m, n);
    }

    a %= m;
    b %= n;
    auto mod = m / g * n;
    auto [x, y] = bezout(m / g, n / g);
    auto X = a * (n / g) % mod * y % mod + b * (m / g) % mod * x % mod;
    if (X < 0) X += mod;
    return X;
}

long long C(long long n, long long k, int p, vector<long long> &fact, vector<long long> &fact_inv) {
    if (k < 0 || k > n) return 0;
    if (n >= p || k >= p) return (C(n / p, k / p, p, fact, fact_inv) * C(n % p, k % p, p, fact, fact_inv)) % p;
    return (fact[n] * fact_inv[k] % p * fact_inv[n - k] % p) % p;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    vector<long long> fact1(MODULO1 + 1, 1), fact2(MODULO2 + 1, 1), fact3(MODULO3 + 1, 1), fact4(MODULO4 + 1, 1),
                fact_inv1(MODULO1 + 1, 1), fact_inv2(MODULO2 + 1, 1), fact_inv3(MODULO3 + 1, 1), fact_inv4(MODULO4 + 1, 1);

    auto prepare = [&](vector<long long> &fact, vector<long long> &fact_inv, int mod) {
        vector<long long> inv(mod, 1);
        for (int i = 2; i < mod; i++) inv[i] = ((mod - mod / i) * inv[mod % i]) % mod;

        for (int i = 1; i <= mod; i++) fact[i] = (i * fact[i - 1]) % mod;
        for (int i = 1; i <= mod; i++) fact_inv[i] = (inv[i] * fact_inv[i - 1]) % mod;
    };

    prepare(fact1, fact_inv1, MODULO1);
    prepare(fact2, fact_inv2, MODULO2);
    prepare(fact3, fact_inv3, MODULO3);
    prepare(fact4, fact_inv4, MODULO4);

    while (t--) {
        long long n, k, c;
        cin >> n >> k >> c;

        auto cgood = 0LL, mod_product = 1LL;
        auto count = [&](long long n, long long c, vector<long long> &fact, vector<long long> &fact_inv, int mod) {
            auto choices = (2 * C(n - 1, c - 1, mod, fact, fact_inv)) % mod;
            cgood = crt(cgood, mod_product, choices, mod);
            mod_product *= mod;
        };

        count(n, c, fact1, fact_inv1, MODULO1);
        count(n, c, fact2, fact_inv2, MODULO2);
        count(n, c, fact3, fact_inv3, MODULO3);
        count(n, c, fact4, fact_inv4, MODULO4);

        cout << cgood << "\n";
    }
}
