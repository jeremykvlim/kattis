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

    long long n;
    cin >> n;

    auto primes = sieve(sqrt(n));

    auto count = [&](auto &&self, int i = 0, long long curr = 1, int sign = 1) -> long long {
        if (i == primes.size()) return sign * n / curr;

        auto next = curr * primes[i];
        if (next > n || next * primes[i] > n) return sign * n / curr;
        return self(self, i + 1, curr, sign) + self(self, i + 1, next * primes[i], -sign);
    };
    cout << count(count);
}
