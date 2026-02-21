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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int na, nb, nc;
    cin >> na >> nb >> nc;

    auto read = [&](int n, auto &pile, auto &count) {
        while (n--) {
            int len;
            cin >> len;

            pile[len] = true;
            count[len]++;
        }
    };

    const int max_len = 3e7 + 1;
    bitset<max_len> A, B, C;
    vector<int> count_a(max_len, 0), count_b(max_len, 0), count_c(max_len, 0);
    read(na, A, count_a);
    read(nb, B, count_b);
    read(nc, C, count_c);

    int s = sqrt(max_len);
    auto spf = sieve(s);
    auto triplets = 0LL;
    for (int m = 2; m < s; m++) {
        vector<int> pfs;
        for (int i = m; i > 1;) {
            int p = spf[i];
            for (; !(i % p); i /= p);
            pfs.emplace_back(p);
        }

        for (int n = (m & 1) + 1; n < m; n += 2) {
            if (any_of(pfs.begin(), pfs.end(), [&](int pf) { return !(n % pf); })) continue;

            int a = m * m - n * n, b = 2 * m * n, c = m * m + n * n;
            for (int k = 1; k * c < max_len; k++) {
                int kc = k * c;
                if (!C[kc]) continue;

                int ka = k * a, kb = k * b;
                bool ab = A[ka] && B[kb], ba = A[kb] && B[ka];
                if (ab || ba) {
                    long long cc = count_c[kc];
                    if (ab) triplets += cc * count_a[ka] * count_b[kb];
                    if (ba) triplets += cc * count_a[kb] * count_b[ka];
                }
            }
        }
    }

    cout << triplets;
}
