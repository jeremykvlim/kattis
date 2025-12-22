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

    int n;
    cin >> n;

    auto spf = sieve(n + 1);
    vector<int> pairs(n + 2, 0);
    pairs[1] = 1;
    for (int c = 2; c <= n + 1; c++) {
        int p = 1;
        for (int d = c; d > 1;) {
            int pf = spf[d], pow = 0;
            for (; !(d % pf); d /= pf, pow++);
            if (pf % 4 == 1) p *= pow + 1;
            else if (pf % 4 == 3 && pow & 1) goto next;
        }
        pairs[c] = p;
        next:;
    }

    int count = 1;
    for (int c = 1; c <= n; c++) {
        auto s = (long long) c * (c + 1) / 2;
        count += (pairs[c] * pairs[c + 1] + (pow(sqrt(s), 2) == s)) / 2;
    }
    cout << count;
}
