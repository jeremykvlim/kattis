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
    return primes;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long m;
    cin >> m;

    auto primes = sieve(ceil(sqrtl(m)));
    long long l = 4, r = m, mid;
    while (l + 1 < r) {
        mid = l + (r - l) / 2;

        auto safe = [&]() {
            int limit = floor(sqrtl(mid));
            for (int p : primes) {
                if (p > limit) break;
                if ((mid / p) + 1 > (m / p)) return false;
            }
            return true;
        };

        if (safe()) l = mid;
        else r = mid;
    }
    cout << l;
}
