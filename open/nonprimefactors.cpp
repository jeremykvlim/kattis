#include <bits/stdc++.h>
using namespace std;

vector<int> sieve(int n) {
    vector<int> npf(n + 1, 0), divisors(n + 1, 0), exponent(n + 1, 0), primes;
    npf[1] = divisors[1] = 1;
    for (int i = 2; i <= n; i++) {
        if (!divisors[i]) {
            divisors[i] = 2;
            npf[i] = exponent[i] = 1;
            primes.emplace_back(i);
        }

        for (int p : primes) {
            auto j = (long long) i * p;
            if (j > n) break;
            if (!(i % p)) {
                exponent[j] = exponent[i] + 1;
                divisors[j] = divisors[i] / (exponent[i] + 1) * (exponent[j] + 1);
                npf[j] = npf[i] + divisors[j] - divisors[i];
                break;
            }
            exponent[j] = 1;
            divisors[j] = divisors[i] * 2;
            npf[j] = npf[i] + divisors[i] - 1;
        }
    }
    return npf;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    auto npf = sieve(2e6);

    int q;
    cin >> q;

    while (q--) {
        int i;
        cin >> i;
        cout << npf[i] << "\n";
    }
}
