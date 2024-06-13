#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 1e9 + 7;

long long mul(long long x, long long y, long long mod) {
    auto product = x * y - mod * (long long) (1.L / mod * x * y);
    return product + mod * (product < 0) - mod * (product >= mod);
}

long long pow(long long base, long long exponent, long long mod) {
    auto value = 1LL;
    for (; exponent; exponent >>= 1) {
        if (exponent & 1) value = mul(value, base, mod);
        base = mul(base, base, mod);
    }

    return value;
}

bool isprime(long long n) {
    if (n < 2) return false;
    if (n % 6 % 4 != 1) return (n | 1) == 3;

    vector<int> bases{2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    int s = __builtin_ctzll(n - 1);
    auto d = n >> s;
    for (int a : bases) {
        auto p = pow(a % n, d, n);
        int i = s;
        while (1 < p && p < n - 1 && a % n && i--) p = mul(p, p, n);
        if (p != n - 1 && i != s) return false;
    }

    return true;
}

long long brent(long long n) {
    if (!(n & 1)) return 2;

    mt19937_64 rng(random_device{}());
    for (;;) {
        auto x = 2LL, y = 2LL, g = 1LL, q = 1LL, xs = -1LL, c = (long long) rng() % (n - 1) + 1;
        for (int i = 1; g == 1; i <<= 1, y = x) {
            for (int j = 1; j < i; j++) x = mul(x, x, n) + c;
            for (int j = 0; j < i && g == 1; j += 128) {
                xs = x;
                for (int k = 0; k < min(128, i - j); k++) {
                    x = mul(x, x, n) + c;
                    q = mul(q, abs(x - y), n);
                }
                g = __gcd(q, n);
            }
        }

        if (g == n) g = 1;
        while (g == 1) {
            xs = mul(xs, xs, n) + c;
            g = __gcd(abs(xs - y), n);
        }
        if (g != n) return g;
    }
}

vector<long long> factorize(long long n) {
    vector<long long> pfs;

    auto dfs = [&](auto &&self, long long m) -> void {
        if (m < 2) return;
        if (isprime(m)) {
            pfs.emplace_back(m);
            return;
        }

        auto d = brent(m);
        self(self, d);
        self(self, m / d);
    };
    dfs(dfs, n);

    return pfs;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long p;
    int n, m;
    cin >> p >> n >> m;

    unordered_map<long long, int> count;
    while (n--) {
        long long g;
        cin >> g;

        auto pfs = factorize(g);
        for (auto pf : pfs) count[pf]--;
    }

    while (m--) {
        long long l;
        cin >> l;

        auto pfs = factorize(l);
        for (auto pf : pfs) count[pf]++;
    }

    auto x = 1LL;
    for (auto [pf, c] : count)
        if (c < 0) {
            cout << 0;
            exit(0);
        } else if (c) x = mul(x, (pow(c + 1, p, MODULO) + 2 * (MODULO - pow(c, p, MODULO)) + pow(c - 1, p, MODULO)) % MODULO, MODULO);

    cout << x;
}
