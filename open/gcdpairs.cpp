#include <bits/stdc++.h>
using namespace std;

vector<int> sieve(int n) {
    if (n < 2) return {};

    int r = sqrt(n);
    vector<int> primes{2};
    vector<bool> composite(n / 2 + 1, false);
    for (int o = 3; o <= r; o += 2)
        if (!composite[o / 2]) {
            for (auto m = (long long) o * o; m <= n; m += 2 * o) composite[m / 2] = true;
            primes.emplace_back(o);
        }

    for (int o = (r + 1) | 1; o <= n; o += 2)
        if (!composite[o / 2]) primes.emplace_back(o);
    return primes;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    auto count = 0LL;
    auto primes = sieve(1e4);
    vector<int> seen(1e6 + 1, 0);
    for (int i = 0; i < n; i++) {
        long long ai;
        cin >> ai;

        vector<int> sq_primes;
        for (int p : primes) {
            int pow = 0;
            while (!(ai % p)) {
                ai /= p;
                pow++;
            }

            if (pow >= 2) sq_primes.emplace_back(p);
        }

        if (ai > 1) {
            int rt = sqrt(ai);
            if ((long long) rt * rt == ai) sq_primes.emplace_back(rt);
        }

        int c = i;
        for (int mask = 1; mask < 1 << sq_primes.size(); mask++) {
            int d = 1;

            for (int j = 0; j < sq_primes.size(); j++)
                if ((mask >> j) & 1) d *= sq_primes[j];

            if (popcount((unsigned) mask) & 1) c -= seen[d];
            else c += seen[d];
            seen[d]++;
        }
        count += c;
    }
    cout << count;
}
