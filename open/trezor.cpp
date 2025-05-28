#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> sieve(int n) {
    vector<int> spf(n + 1, 0), primes;
    vector<vector<int>> pfs(n + 1);
    for (int i = 2; i <= n; i++) {
        if (!spf[i]) {
            spf[i] = i;
            primes.emplace_back(i);
            pfs[i].emplace_back(i);
        }

        for (int p : primes) {
            auto j = (long long) i * p;
            if (j > n) break;

            spf[j] = p;
            pfs[j] = pfs[i];
            if (pfs[j].back() != p) pfs[j].emplace_back(p);
            if (p == spf[i]) break;
        }
    }

    return pfs;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int a, b, l;
    cin >> a >> b >> l;

    auto pfs = sieve(a + b);

    auto coprimes = [l](const vector<int> &factors) {
        vector<array<int, 2>> p{{1, 1}};
        for (int pf : factors) {
            int s = p.size();
            for (int j = 0; j < s; j++) p.push_back({p[j][0] * pf, -p[j][1]});
        }

        int count = 0;
        for (auto [d, sgn] : p) count += sgn * l / d;
        return count;
    };

    long long secure = 2 * coprimes(pfs[a + b]) - 2, insecure = 2 * l - secure - 2, supersecure = 2;
    for (int x = 1, y = a + b - 1; x < a + b && y; x++, y--) {
        auto fx = pfs[x], fy = pfs[y];
        sort(fx.begin(), fx.end());
        sort(fy.begin(), fy.end());
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
