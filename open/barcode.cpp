#include <bits/stdc++.h>
using namespace std;

long long C(long long n, long long k, int p, vector<long long> &fact, vector<long long> &fact_inv) {
    if (k < 0 || k > n) return 0;
    if (n >= p || k >= p) return (C(n / p, k / p, p, fact, fact_inv) * C(n % p, k % p, p, fact, fact_inv)) % p;
    return (fact[n] * fact_inv[k] % p * fact_inv[n - k] % p) % p;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n, m;
        cin >> n >> m;

        vector<long long> fact(n + 1, 1), fact_inv(n / 2 + 1, 1);
        auto prepare = [&](vector<long long> &fact, vector<long long> &fact_inv, int mod) {
            vector<long long> inv(n / 2 + 1, 1);

            for (int i = 1; i <= n / 2; i++) {
                if (i > 1) inv[i] = ((mod - mod / i) * inv[mod % i]) % mod;
                fact_inv[i] = (inv[i] * fact_inv[i - 1]) % mod;
            }

            for (int i = 1; i <= n; i++) fact[i] = (i * fact[i - 1]) % mod;
        };

        prepare(fact, fact_inv, m);
        
        auto count = 0LL;
        if (!(n & 1)) {
            count -= (n / 2 + 1) % m;
            count = (count + C(n, n / 2, m, fact, fact_inv)) % m;
        }

        int fib1 = 0, fib2 = 1;
        while (n--) {
            fib1 = (fib1 + fib2) % m;
            swap(fib1, fib2);
        }

        cout << (count + fib1 + fib2 + m) % m << "\n";
    }
}
