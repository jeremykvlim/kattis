#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;

struct Hash {
    size_t operator()(const int &i) const {
        return 0ULL ^ (i + 0x9e3779b9 + (i << 6) + (i >> 2));
    }
};

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
        auto x = 2LL, y = 2LL, g = 1LL, q = 1LL, xs = 1LL, c = (long long) rng() % (n - 1) + 1;
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
        if (g != n) return isprime(g) ? g : brent(g);
    }
}

vector<pair<long long, int>> factorize(long long n, vector<int> &spf) {
    gp_hash_table<long long, int, Hash> pfs;

    auto dnc = [&](auto &&self, long long m) -> void {
        if (m < 2) return;
        if (isprime(m) || m < spf.size() && spf[m] == m) {
            pfs[m]++;
            return;
        }

        if (m <= 1e7) {
            while (m > 1) {
                pfs[spf[m]]++;
                m /= spf[m];
            }
            return;
        }

        auto d = brent(m);
        self(self, d);
        self(self, m / d);
    };
    dnc(dnc, n);

    return {pfs.begin(), pfs.end()};
}

void divisors(vector<pair<long long, int>> &pfs, vector<long long> &divs, long long d = 1, int i = 0) {
    if (i == pfs.size()) return;

    divisors(pfs, divs, d, i + 1);

    auto [pf, pow] = pfs[i];
    while (pow--) {
        d *= pf;
        divs.emplace_back(d);
        divisors(pfs, divs, d, i + 1);
    }
}

long long phi(long long n, vector<pair<long long, int>> pfs) {
    auto count = n;
    for (auto [pf, pow] : pfs) count -= count / pf;
    return count;
}

vector<int> sieve(int n) {
    vector<int> spf(n + 1, 0);
    for (int i = 2; i <= n; i++)
        if (!spf[i])
            for (int j = i; j <= n; j += i)
                if (!spf[j]) spf[j] = i;

    return spf;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    auto spf = sieve(1e7);
    while (t--) {
        long long n;
        cin >> n;

        auto pfs = factorize(n, spf);
        vector<long long> divs{1};
        divisors(pfs, divs);

        auto sum = 0LL;
        for (auto d : divs) sum += phi(d + 1, factorize(d + 1, spf));
        cout << sum << "\n";
    }
}
