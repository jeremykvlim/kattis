#include <bits/stdc++.h>
using namespace std;

vector<int> sieve(int n) {
    vector<int> factors(n + 1), primes{2};
    iota(factors.begin(), factors.end(), 0);
    for (int p = 3; p <= n; p += 2)
        if (factors[p] == p) {
            primes.emplace_back(p);
            for (auto i = (long long) p * p; i <= n; i += 2 * p)
                if (factors[i] == i) factors[i] = p;
        }

    return primes;
}

int find(int p, vector<int> &sets) {
    return (sets[p] == p) ? p : (sets[p] = find(sets[p], sets));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    vector<int> primes = sieve(1e6);
    for (int x = 1; x <= t; x++) {
        long long a, b, p;
        cin >> a >> b >> p;

        long long size = b - a + 1;
        vector<int> sets(size);
        iota(sets.begin(), sets.end(), 0);

        for (int i = primes.size() - 1; i >= 0 && primes[i] >= p; i--) {
            auto l = (primes[i] - a % primes[i]) % primes[i];
            for (auto r = l + primes[i]; r <= b - a; r += primes[i]) {
                int l_set = find(l, sets), r_set = find(r, sets);
                if (l_set != r_set) {
                    size--;
                    sets[r_set] = sets[l_set];
                }
            }
        }

        cout << "Case #" << x << ": " << size << "\n";
    }
}
