#include <bits/stdc++.h>
using namespace std;

template <typename T, typename U, typename V>
T mul(U x, V y, T mod) {
    return (unsigned __int128) x * y % mod;
}

template <typename T, typename U>
T pow(T base, U exponent, T mod) {
    T value = 1;
    while (exponent) {
        if (exponent & 1) value = mul(base, value, mod);
        base = mul(base, base, mod);
        exponent >>= 1;
    }
    return value;
}

bool isprime(unsigned long long n) {
    if (n < 2) return false;
    if (n == 2 || n == 5 || n == 11) return true;
    if (n % 6 % 4 != 1) return (n | 1) == 3;

    auto miller_rabin = [&](int a) {
        int s = countr_zero(n - 1);
        auto d = n >> s, x = pow(a % n, d, n);
        if (x == 1 || x == n - 1) return true;

        while (s--) {
            x = mul(x, x, n);
            if (x == n - 1) return true;
        }
        return false;
    };
    if (!miller_rabin(2) || !miller_rabin(3)) return false;

    auto lucas_pseudoprime = [&]() {
        auto normalize = [&](__int128 &x) {
            if (x < 0) x += ((-x / n) + 1) * n;
        };

        __int128 D = -3;
        for (;;) {
            D += D > 0 ? 2 : -2;
            D *= -1;

            int jacobi = 1;
            auto jacobi_symbol = [&](__int128 n) {
                auto a = D;
                normalize(a);

                while (a) {
                    while (!(a & 1)) {
                        a >>= 1;
                        if ((n & 7) == 3 || (n & 7) == 5) jacobi = -jacobi;
                    }
                    if ((a & 3) == 3 && (n & 3) == 3) jacobi = -jacobi;

                    swap(a, n);
                    a %= n;
                }
                return n == 1;
            };
            if (!jacobi_symbol(n)) return false;
            if (jacobi == -1) break;
        }

        string bits;
        auto temp = n + 1;
        while (temp) {
            bits += (temp & 1) ? '1' : '0';
            temp >>= 1;
        }
        bits.pop_back();
        reverse(bits.begin(), bits.end());

        auto div2mod = [&](__int128 x) -> unsigned long long {
            if (x & 1) x += n;
            normalize(x >>= 1);

            return x % n;
        };

        __int128 U = 1, V = 1;
        for (char b : bits) {
            auto U_2k = mul(U, V, n), V_2k = div2mod(mul(V, V, n) + D * mul(U, U, n));

            if (b == '0') {
                U = U_2k;
                V = V_2k;
            } else {
                U = div2mod(U_2k + V_2k);
                V = div2mod(D * U_2k + V_2k);
            }
        }

        return !U;
    };
    return lucas_pseudoprime();
}

vector<vector<int>> sieve(int n) {
    vector<int> spf(n + 1, 0), primes;
    vector<vector<int>> pfs(n + 1);
    for (int i = 2; i <= n; i++) {
        if (!spf[i]) {
            spf[i] = i;
            primes.emplace_back(i);
            pfs[i].emplace_back(i);
        }

        for (int p : primes) {
            auto j = (long long) i * p;
            if (j > n) break;

            spf[j] = p;
            pfs[j] = pfs[i];
            if (pfs[j].back() != p) pfs[j].emplace_back(p);
            if (p == spf[i]) break;
        }
    }

    return pfs;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    auto pfs = sieve(1e4 + 7);
    vector<bool> visited(1e4 + 8, false);
    vector<vector<int>> dp(1e4 + 8, vector<int>(3, 1e9));
    for (int p = 2, prev = 1; p <= 1e4 + 7; p == 2 ? p++ : p += 2) {
        if (!isprime(p)) continue;
        dp[p][0] = 1;
        for (int i = p - 1; i > prev; i--) {
            if (!visited[i]) sort(pfs[i].begin(), pfs[i].end());
            visited[i] = true;
            int j = i + 1;
            dp[i][0] = dp[j][2];
            for (int pf : pfs[i])
                if (dp[i][0] >= min(i / pf, dp[i / pf][2])) {
                    dp[i][0] = min(i / pf, dp[i / pf][2]);
                    j = i / pf;
                }
            dp[i][1] = dp[j][0];
            dp[i][2] = dp[j][1];
        }
        prev = p;
    }

    int n;
    cin >> n;

    unordered_map<char, int> indices{{'O', 0}, {'E', 1}, {'I', 2}};
    vector<int> scores(3, 0);
    while (n--) {
        char c;
        int i;
        cin >> c >> i;

        for (int j = 0; j < 3; j++) scores[(indices[c] + j) % 3] += min(i, dp[i][j]);
    }

    for (int s : scores) cout << s << " ";
}
