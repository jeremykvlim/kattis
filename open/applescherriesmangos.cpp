#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 1e9 + 7;

long long C(int n, int k, int p, vector<long long> &fact, vector<long long> &fact_inv) {
    if (k < 0 || k > n) return 0;
    if (n >= p || k >= p) return (C(n / p, k / p, p, fact, fact_inv) * C(n % p, k % p, p, fact, fact_inv)) % p;
    return (fact[n] * fact_inv[k] % p * fact_inv[n - k] % p) % p;
}

long long P(int n, int k1, int k2, int k3, int p, vector<long long> &fact, vector<long long> &fact_inv) {
    if (k1 < 0 || k1 > n || k2 < 0 || k2 > n || k3 < 0 || k3 > n) return 0;
    if (n >= p || k1 >= p || k2 >= p || k3 >= p) return (P(n / p, k1 / p, k2 / p, k3 / p, p, fact, fact_inv) * P(n % p, k1 % p, k2 % p, k3 % p, p, fact, fact_inv)) % p;
    return (fact[n] * fact_inv[k1] % p * fact_inv[k2] % p * fact_inv[k3] % p) % p;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int a, c, m;
    cin >> a >> c >> m;

    if (a < m) swap(a, m);
    if (a < c) swap(a, c);
    if (c < m) swap(c, m);

    vector<long long> fact(a + c + m + 1, 1), fact_inv(a + c + m + 1, 1), p2(a + c + m + 1, 1);
    auto prepare = [&](vector<long long> &fact, vector<long long> &fact_inv, vector<long long> &p2, int mod) {
        vector<long long> inv(a + c + m + 1, 1);
        for (int i = 2; i <= a + c + m; i++) inv[i] = ((mod - mod / i) * inv[mod % i]) % mod;

        for (int i = 1; i <= a + c + m; i++) {
            fact[i] = (i * fact[i - 1]) % mod;
            fact_inv[i] = (inv[i] * fact_inv[i - 1]) % mod;
            p2[i] = (p2[i - 1] * 2) % mod;
        }
    };

    prepare(fact, fact_inv, p2, MODULO);

    auto ways = [&](int a) {
        auto w = 0LL;
        for (int i = 0; 2 * i <= a - c + m; i++) {
            int j = i + c - m, k = a - i - j;
            if (m - i - k < 0) continue;

            w = (w + P(i + j + k, i, j, k, MODULO, fact, fact_inv) * C(m - i - k + a - 1, a - 1, MODULO, fact, fact_inv) % MODULO * p2[k] % MODULO) % MODULO;
        }

        return w;
    };

    cout << ((2 * ways(a) % MODULO + ways(a + 1)) % MODULO + ways(a - 1)) % MODULO;
}
