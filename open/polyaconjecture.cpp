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

    int d;
    cin >> d;

    auto spf = sieve(1e6);

    vector<int> lambda(1e6 + 1);
    int s = lambda[1] = 1;
    for (int n = 2; n <= 1e6; n++) {
        lambda[n] = -lambda[n / spf[n]];
        s += lambda[n];
        if (s == -d) {
            cout << n;
            exit(0);
        }
    }
}
