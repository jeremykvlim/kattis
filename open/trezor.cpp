#include <bits/stdc++.h>
using namespace std;

vector<int> sieve(int n) {
    vector<int> spf(n + 1, 0), primes;
    for (int i = 2; i <= n; i++) {
        if (!spf[i]) {
            spf[i] = i;
            primes.emplace_back(i);
        }

        for (int p : primes) {
            auto j = (long long) i * p;
            if (j > n) break;
            spf[j] = p;
            if (p == spf[i]) break;
        }
    }
    return spf;
}

vector<vector<int>> generate_prime_factors(int n, const vector<int> &spf) {
    vector<vector<int>> pfs(n + 1);
    for (int i = 2; i <= n; i++)
        for (int j = i; j > 1;) {
            int p = spf[j];
            for (; !(j % p); j /= p);
            pfs[i].emplace_back(p);
        }
    return pfs;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int a, b, l;
    cin >> a >> b >> l;
    int n = a + b;

    auto pfs = generate_prime_factors(n, sieve(n));

    auto coprimes = [l](const vector<int> &factors) {
        int count = 0;
        for (int mask = 0; mask < 1 << (factors.size()); mask++) {
            int bits = 0, d = 1;
            for (int i = 0; i < factors.size(); i++)
                if ((mask >> i) & 1) {
                    bits++;
                    d *= factors[i];
                    if (l < d) break;
                }
            if (l >= d) count += (bits & 1 ? -1 : 1) * (l / d);
        }
        return count;
    };

    long long secure = 2 * coprimes(pfs[n]) - 2, insecure = 2 * l - secure - 2, supersecure = 2;
    for (int x = 1, y = n - 1; x < n && y; x++, y--) {
        auto fx = pfs[x], fy = pfs[y];
        int c1 = coprimes(fx), c2 = coprimes(fy);

        vector<int> fxy;
        set_union(fx.begin(), fx.end(), fy.begin(), fy.end(), back_inserter(fxy));

        int c3 = coprimes(fxy);
        secure += c1 + c2 - 2 * c3;
        insecure += l - c1 - c2 + c3;
        supersecure += c3;
    }
    cout << insecure << "\n" << secure << "\n" << supersecure;
}
