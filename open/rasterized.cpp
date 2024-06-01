#include <bits/stdc++.h>
using namespace std;

vector<pair<long long, int>> prime_factors(long long n, vector<int> primes) {
    vector<pair<long long, int>> pfs;
    for (int i = 0; i < primes.size() && primes[i] <= sqrt(n); i++)
        if (!(n % primes[i])) {
            int pow = 0;
            while (!(n % primes[i])) {
                n /= primes[i];
                pow++;
            }
            pfs.emplace_back(primes[i], pow);
        }

    if (n > 1) pfs.emplace_back(n, 1);

    return pfs;
}

void divisors(vector<pair<long long, int>> &pfs, vector<long long> &divs, long long d = 1, int i = 0) {
    if (i == pfs.size()) return;

    divisors(pfs, divs, d, i + 1);

    auto [pf, pow] = pfs[i];
    while (pow--) {
        d *= pf;
        divs.emplace_back(d);
        divisors(pfs, divs, d, i + 1);
    }
}

long long phi(long long n, vector<pair<long long, int>> pfs) {
    auto count = n;

    for (auto [pf, pow] : pfs) {
        count /= pf;
        count *= pf - 1;
    }

    return count;
}

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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    auto primes = sieve(1e7);
    while (t--) {
        long long n;
        cin >> n;

        auto pfs = prime_factors(n, primes);
        vector<long long> divs{1};
        divisors(pfs, divs);

        auto sum = 0LL;
        for (auto d : divs) sum += phi(d + 1, prime_factors(d + 1, primes));
        cout << sum << "\n";
    }
}
