#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO1 = 29, MODULO2 = 34483;

long long C(int n, int m, int p, vector<int> &fact, vector<int> &fact_inv) {
    if (m < 0 || m > n) return 0;
    if (n >= p || m >= p) return (C(n / p, m / p, p, fact, fact_inv) * C(n % p, m % p, p, fact, fact_inv)) % p;
    return (fact[n] * fact_inv[m] % p * fact_inv[n - m] % p) % p;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    vector<int> fact1(MODULO1 + 1, 1), fact2(MODULO2 + 1, 1), fact_inv1(MODULO1 + 1, 1), fact_inv2(MODULO2 + 1, 1);
    auto prepare = [&](vector<int> &fact, vector<int> &fact_inv, int mod) {
        vector<int> inv(mod, 1);
        for (int i = 2; i < mod; i++) inv[i] = ((mod - mod / i) * inv[mod % i]) % mod;

        for (int i = 1; i <= mod; i++) {
            fact[i] = (i * fact[i - 1]) % mod;
            fact_inv[i] = (inv[i] * fact_inv[i - 1]) % mod;
        }
    };

    prepare(fact1, fact_inv1, MODULO1);
    prepare(fact2, fact_inv2, MODULO2);

    auto ways = [&](int mod, vector<int> &fact, vector<int> &fact_inv) -> long long {
        auto w = 0LL;
        for (int i = 0; i * (m + 1) <= k; i++) {
            auto c = (C(n, i, mod, fact, fact_inv) * C(n + k - i * (m + 1) - 1, k - i * (m + 1), mod, fact, fact_inv)) % mod;
            i & 1 ? w -= c : w += c;
        }
        w %= mod;
        if (w < 0) w += mod;

        return w;
    };

    auto ways1 = ways(MODULO1, fact1, fact_inv1), ways2 = ways(MODULO2, fact2, fact_inv2);
    while (ways2 % MODULO1 != ways1) ways2 += MODULO2;
    cout << ways2;
}
