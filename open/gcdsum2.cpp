#include <bits/stdc++.h>
using namespace std;

vector<int> totients(int n) {
    vector<bool> prime(n + 1, true);
    vector<int> phis(n + 1, 0), primes;
    for (int i = 2; i <= n; i++) {
        if (prime[i]) {
            primes.emplace_back(i);
            phis[i] = i - 1;
        }

        for (int j = 0; primes[j] <= n / i; j++) {
            int k = i * primes[j];
            prime[k] = false;

            if (!(i % primes[j])) {
                phis[k] = phis[i] * primes[j];
                break;
            }
            phis[k] = phis[i] * phis[primes[j]];
        }
    }

    return phis;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    auto phis = totients(n);

    vector<long long> pref{phis.begin(), phis.end()};
    for (int i = 1; i <= n; i++) pref[i] += pref[i - 1];

    auto sum = 0LL;
    for (int l = 1, r; l <= n; l = r + 1) {
        r = n / (n / l);
        sum += pref[n / l] * (l + r) * (r - l + 1) / 2;
    }

    cout << sum;
}
