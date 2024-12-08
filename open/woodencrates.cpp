#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;

struct Hash {
    size_t operator()(const int &i) const {
        return 0ULL ^ (i + 0x9e3779b9 + (i << 6) + (i >> 2));
    }
};

template <typename T, typename U, typename V>
T mul(U x, V y, T mod) {
    return ((unsigned __int128) x * y) % mod;
}

template <typename T, typename U>
T pow(T base, U exponent, T mod) {
    T value = 1;
    while (exponent) {
        if (exponent & 1) value = mul(value, base, mod);
        base = mul(base, base, mod);
        exponent >>= 1;
    }
    return value;
}

bool isprime(unsigned long long n) {
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

template <typename T>
T brent(T n) {
    if (!(n & 1)) return 2;

    mt19937_64 rng(random_device{}());
    for (;;) {
        T x = 2, y = 2, g = 1, q = 1, xs = 1, c = rng() % (n - 1) + 1;
        for (int i = 1; g == 1; i <<= 1, y = x) {
            for (int j = 1; j < i; j++) x = mul(x, x, n) + c;
            for (int j = 0; j < i && g == 1; j += 128) {
                xs = x;
                for (int k = 0; k < min(128, i - j); k++) {
                    x = mul(x, x, n) + c;
                    q = mul(q, max(x, y) - min(x, y), n);
                }
                g = __gcd(q, n);
            }
        }

        if (g == n) g = 1;
        while (g == 1) {
            xs = mul(xs, xs, n) + c;
            g = __gcd(max(xs, y) - min(xs, y), n);
        }
        if (g != n) return isprime(g) ? g : brent(g);
    }
}

template <typename T>
vector<pair<T, int>> factorize(T n) {
    gp_hash_table<T, int, Hash> pfs;

    auto dnc = [&](auto &&self, T m) -> void {
        if (m < 2) return;
        if (isprime(m)) {
            pfs[m]++;
            return;
        }

        auto d = brent(m);
        self(self, d);
        self(self, m / d);
    };
    dnc(dnc, n);

    return {pfs.begin(), pfs.end()};
}

template <typename T>
vector<T> divisors(T n) {
    auto pfs = factorize(n);
    vector<T> divs{1};

    auto dfs = [&](auto &&self, T d = 1, int i = 0) {
        if (i == pfs.size()) return;

        self(self, d, i + 1);
        auto [pf, pow] = pfs[i];
        while (pow--) {
            d *= pf;
            divs.emplace_back(d);
            self(self, d, i + 1);
        }
    };
    dfs(dfs);

    return divs;
}

long long rearrange(long long d, long long total, vector<int> crates) {
    auto actions = 0LL, average = total / d;
    for (int c : crates) actions += abs(c - average);
    actions += average * (d - crates.size());

    auto last = d - 1;
    if (d == crates.size()) {
        while (~last && crates[last] == average) last--;
        if (last == -1) return 0;
    }

    vector<long long> diff(min((long long) crates.size(), last), -average);
    for (int i = 0; i < diff.size(); i++) {
        diff[i] += crates[i];
        if (i) diff[i] += diff[i - 1];
    }

    if (crates.size() < last) actions += (diff.back() - average) * ((diff.back() - average) / average + 1) - last + crates.size();

    for (auto &di : diff) {
        actions += abs(di);
        if (di > 1) {
            actions += di - 1;
            di = 1;
        } else if (!di) {
            actions++;
            di = 1;
        } else if (di < 0) {
            actions -= di;
            di = 0;
        }
    }

    long long remaining1 = count(diff.begin(), diff.end(), 0), remaining2 = last - remaining1;
    remaining1 = min(remaining1, remaining2);
    for (auto di : diff) {
        remaining2 += !di - (di == 1);
        remaining1 = min(remaining1, remaining2);
    }

    actions += remaining1;
    return actions;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> crates(n);
    for (int &c : crates) cin >> c;

    auto total = accumulate(crates.begin(), crates.end(), 0LL), actions = LLONG_MAX;
    auto divs = divisors(total);
    for (auto d : divs)
        if (d >= n) actions = min(actions, rearrange(d, total, crates));

    cout << actions;
}
