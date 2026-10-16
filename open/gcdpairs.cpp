#include <bits/stdc++.h>
#include <dlfcn.h>
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

    struct mpz_t {
        int _mp_alloc, _mp_size;
        unsigned long int *_mp_d;
    };
    void *gmp = dlopen("libgmp.so.10", RTLD_NOW);
    auto init = (void (*)(mpz_t *x)) dlsym(gmp, "__gmpz_init");
    auto priomorial_ui = (void (*)(mpz_t *rop, unsigned long int n)) dlsym(gmp, "__gmpz_primorial_ui");
    auto gcd_ui = (unsigned long int (*)(mpz_t *rop, const mpz_t *op1, unsigned long int op2)) dlsym(gmp, "__gmpz_gcd_ui");
    auto perfect_square_p = (long int (*)(const long unsigned int *s1p, long int n)) dlsym(gmp, "__gmpn_perfect_square_p");

    mpz_t product;
    init(&product);
    priomorial_ui(&product, 10000);

    int n;
    cin >> n;

    int m = 1;
    vector<int> freq(1e6 + 1, 0);
    for (int i = 0; i < n; i++) {
        unsigned long a;
        cin >> a;

        auto d = gcd_ui(nullptr, &product, a);
        int g = 1;
        if (d > 1) {
            a /= d;
            g = gcd(a, d);
            for (int temp = g; temp > 1; temp = gcd(a, d)) a /= temp;
        }

        if (perfect_square_p(&a, 1)) g *= sqrt(a);
        freq[g]++;
        m = max(m, g);
    }

    auto spf = sieve(m);
    auto factorize = [&](int n) {
        vector<int> pfs;
        while (n > 1) {
            int pf = spf[n];
            pfs.emplace_back(pf);
            while (!(n % pf)) n /= pf;
        }
        return pfs;
    };

    vector<int> count(m + 1, 0);
    for (int g = 2; g <= m; g++)
        if (freq[g]) {
            auto pfs = factorize(g);
            vector<int> products(1 << pfs.size(), 0);
            products[0] = 1;
            for (int mask = 1; mask < 1 << pfs.size(); mask++) {
                int i = countr_zero((unsigned) mask);
                count[products[mask] = products[mask ^ (1 << i)] * pfs[i]] += (popcount((unsigned) mask) & 1 ? -1 : 1) * freq[g];
            }
        }

    auto c = (long long) n * (n - 1) / 2;
    for (int p = 2; p <= m; p++) c += (long long) count[p] * (abs(count[p]) - 1) / 2;
    cout << c;
}
