#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 1e9 + 7;

vector<bool> sieve(int n) {
    vector<int> factors(n + 1);
    vector<bool> primes(n + 1, false);
    primes[2] = true;
    iota(factors.begin(), factors.end(), 0);
    for (int p = 3; p <= n; p += 2)
        if (factors[p] == p) {
            primes[p] = true;
            for (auto i = (long long) p * p; i <= n; i += 2 * p)
                if (factors[i] == i) factors[i] = p;
        }

    return primes;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> seq(n);
    for (int &a : seq) cin >> a;

    auto primes = sieve(*max_element(seq.begin(), seq.end()));
    unordered_map<int, int> pfs;
    for (int a : seq) {
        if (a == 1) continue;

        if (primes[a]) pfs[a]++;
        else {
            for (int p = 2; p <= sqrt(a); p == 2 ? p++ : p += 2)
                while (!(a % p)) {
                    pfs[p]++;
                    a /= p;
                }
            if (a > 1) pfs[a]++;
        }
    }

    auto d = 1LL;
    for (auto [pf, pow] : pfs) d = (d * (pow + 1)) % MODULO;

    cout << d;
}
