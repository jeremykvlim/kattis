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

    vector<int> blocks{124, 74, 64, 68, 61, 59, 58, 52, 57, 51, 52, 59, 43, 49, 46, 60, 53, 51, 45, 41, 38, 57, 48, 59,
                       51, 51, 53, 43, 50, 48, 37, 37, 51, 44, 44, 61, 43, 47, 33, 45, 53, 40, 48, 46, 55, 42, 44, 43,
                       48, 50, 38, 48, 45, 45, 34, 38, 50, 37, 43, 50, 44, 50, 43, 40, 46, 38, 47, 43, 46, 42, 43, 51,
                       44, 35, 46, 43, 48, 44, 43, 55, 47, 46, 36, 39, 44, 40, 40, 34, 41, 42, 32, 50, 42, 38, 35, 40,
                       44, 39, 40, 44, 46, 40, 33, 31, 34, 48, 41, 35, 36, 46, 40, 46, 49, 43, 46, 42, 38, 44, 36, 47,
                       39, 48, 34, 44, 43, 39, 50, 46, 45, 41, 43, 43, 51, 36, 45, 45, 35, 34, 37, 45, 41, 39, 48, 45,
                       43, 48, 47, 32, 45, 36, 36, 44, 51, 49, 38, 47, 42, 37, 47, 30, 44, 36, 40, 40, 43, 41, 35, 39,
                       44, 42, 54, 48, 39, 41, 35, 39, 51, 46, 35, 44, 34, 52, 43, 40, 38, 35, 38, 38, 41, 45, 59, 48,
                       44, 34, 49, 38, 35, 43, 41, 44, 39, 42, 44, 40, 35, 26, 44, 40, 29, 41, 33, 43, 22, 41, 48, 46,
                       35, 42, 38, 37, 43, 45, 37, 43, 35, 44, 35, 46, 45, 43, 41, 34, 33, 33, 54, 40, 30, 33, 38, 45,
                       30, 35, 37, 41, 40, 38, 46, 36, 36, 45};

    int a, b;
    cin >> a >> b;
    a += !(a & 1);

    auto primes = sieve(1e5);
    auto factorize = [&](int n) {
        unordered_map<int, int> pfs;
        for (int p : primes) {
            if (p * p > n) break;
            while (!(n % p)) {
                pfs[p]++;
                n /= p;
            }
        }
        if (n > 1) pfs[n]++;
        return pfs;
    };

    int size = 1e8 / blocks.size(), count = 0;
    for (int n = a; n <= b;) {
        if (n % size == 1 && n + size < b) {
            count += blocks[n / size];
            n += size;
            continue;
        }

        unordered_map<int, int> pow_count;
        for (auto [pf, pow] : factorize(n)) pow_count[pow]++;

        unordered_map<int, int> pfs;
        for (auto [pow, freq] : pow_count)
            for (auto [pf1, pow1] : factorize(n * pow + 1)) {
                if (freq > 1) pow1 *= freq;
                pfs[pf1] += pow1;
            }

        int product = 1;
        for (auto [pf, pow] : pfs) product *= pow + 1;
        count += !(n % product);
        n += 2;
    }
    cout << count;
}
