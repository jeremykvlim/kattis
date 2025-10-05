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

template <typename T>
T brent(T n) {
    if (!(n & 1)) return 2;

    mt19937_64 rng(random_device{}());
    for (;;) {
        T x = 2, y = 2, g = 1, q = 1, xs = 1, c = rng() % (n - 1) + 1;
        for (int i = 1; g == 1; i <<= 1, y = x) {
            for (int j = 1; j < i; j++) x = mul(x, x, n) + c;
            for (int j = 0; j < i && g == 1; j += 128) {
                xs = x;
                for (int k = 0; k < min(128, i - j); k++) {
                    x = mul(x, x, n) + c;
                    q = mul(q, max(x, y) - min(x, y), n);
                }
                g = __gcd(q, n);
            }
        }

        if (g == n) g = 1;
        while (g == 1) {
            xs = mul(xs, xs, n) + c;
            g = __gcd(max(xs, y) - min(xs, y), n);
        }
        if (g != n) return isprime(g) ? g : brent(g);
    }
}

template <typename T>
vector<pair<T, int>> factorize(T n) {
    unordered_map<T, int> pfs;

    auto dfs = [&](auto &&self, T m) -> void {
        if (m < 2) return;
        if (isprime(m)) {
            pfs[m]++;
            return;
        }

        auto pf = brent(m);
        pfs[pf]++;
        self(self, m / pf);
    };
    dfs(dfs, n);

    return {pfs.begin(), pfs.end()};
}

template <typename T>
vector<T> divisors(T n) {
    auto pfs = factorize(n);
    vector<T> divs{1};

    auto dfs = [&](auto &&self, T d = 1, int i = 0) {
        if (i == pfs.size()) return;

        self(self, d, i + 1);
        auto [pf, pow] = pfs[i];
        while (pow--) {
            d *= pf;
            divs.emplace_back(d);
            self(self, d, i + 1);
        }
    };
    dfs(dfs);

    return divs;
}

vector<int> sieve(int n) {
    vector<int> spf(n + 1, 0), primes, mobius(n + 1, 1);
    for (int i = 2; i <= n; i++) {
        if (!spf[i]) {
            spf[i] = i;
            primes.emplace_back(i);
            mobius[i] = -1;
        }

        for (int p : primes) {
            auto j = (long long) i * p;
            if (j > n) break;

            spf[j] = p;
            if (!(i % p)) {
                mobius[j] = 0;
                break;
            } else mobius[j] = -mobius[i];
        }
    }
    return mobius;
}

vector<int> totients(int n) {
    vector<bool> prime(n + 1, true);
    vector<int> phis(n + 1, 0), primes;
    phis[1] = 1;
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

istream & operator>>(istream &stream, __int128 &v) {
    string s;
    stream >> s;

    v = 0;
    for (char c : s)
        if (isdigit(c)) v = v * 10 + c - '0';
    if (s[0] == '-') v *= -1;
    return stream;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    auto impossible = [&]() {
        cout << "impossible";
        exit(0);
    };

    vector<int> periods(k);
    for (int &p : periods) cin >> p;

    vector<__int128> m(k);
    unordered_map<int, __int128> count;
    for (int i = 0; i < k; i++) {
        cin >> m[i];

        count[periods[i]] = m[i];
    }
    if (accumulate(m.begin(), m.end(), (__int128) 0) != ((__int128) 1) << n) impossible();

    vector<int> w(n + 1, 0);
    for (int p = 1; p <= n; p++) {
        unsigned __int128 c = 0;
        for (int i = 0; i < k; i++)
            if (!(p % periods[i])) c += m[i];

        if (!(c > 1 && has_single_bit(c))) impossible();
        w[p] = bit_width(c) - 1;
    }

    auto phis = totients(n);
    vector<int> cycles_total(n + 1, 0);
    for (int p = 1; p <= n; p++) {
        for (int i = 1; i < p; i++)
            if (!(p % i)) w[p] -= phis[i] * cycles_total[i];
        if (w[p] < 0 || w[p] % phis[p]) impossible();
        cycles_total[p] = w[p] / phis[p];
    }

    auto mobius = sieve(n);
    vector<int> cycles_per_l(n + 1, 0);
    for (int i = 1; i <= n; i++)
        if (mobius[i])
            for (int l = 1; l * i <= n; l++) cycles_per_l[l] += mobius[i] * cycles_total[l * i];

    int len = 0;
    for (int l = 1; l <= n; l++) {
        if (cycles_per_l[l] < 0) impossible();
        len += l * cycles_per_l[l];
    }
    if (len != n) impossible();

    for (int p : periods) {
        auto divs = divisors(p);
        unordered_map<int, __int128> check;
        for (int d1 : divs) {
            __int128 c = 1;
            for (int l = 1; l <= n; l++)
                if (cycles_per_l[l]) c <<= cycles_per_l[l] * gcd(l, d1);
            for (int d2 : divs) {
                if (d1 == d2) break;
                if (!(d1 % d2)) c -= check[d2];
            }
            check[d1] = c;
        }
        if (check[p] != count[p]) impossible();
    }

    vector<int> f(n);
    for (int l = 1, sum = 1; l <= n; l++)
        while (cycles_per_l[l]--) {
            for (int i = 0; i < l; i++) f[sum + i - 1] = sum + (i + 1) % l;
            sum += l;
        }
    for (int fi : f) cout << fi << " ";
}
