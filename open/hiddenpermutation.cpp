#include <bits/stdc++.h>
using namespace std;

bool isprime(unsigned long long n) {
    if (n < 2) return false;
    if (n == 2 || n == 5 || n == 11) return true;
    if (n % 6 % 4 != 1) return (n | 1) == 3;

    auto inv = n, one = -n % n;
    while (n * inv != 1) inv *= 2 - n * inv;

    auto add = [&](auto x, auto y) {
        return x >= n - y ? x - (n - y) : x + y;
    };

    auto sub = [&](auto x, auto y) {
        return x >= y ? x - y : n - (y - x);
    };

    auto mul = [&](auto x, auto y) -> unsigned long long {
        auto z = (unsigned __int128) x * y;
        unsigned long long q = z * inv;
        auto v = (z >> 64) + n - (((unsigned __int128) q * n) >> 64);
        return v >= n ? v - n : v;
    };

    auto pow = [&](auto base, auto exponent) {
        auto value = one;
        while (exponent) {
            if (exponent & 1) value = mul(value, base);
            base = mul(base, base);
            exponent >>= 1;
        }
        return value;
    };

    auto two = add(one, one);
    auto miller_rabin = [&](auto a) {
        int s = countr_zero(n - 1);
        auto x = pow(a, (n - 1) >> s);
        if (x == one || x == n - one) return true;

        while (--s) {
            x = mul(x, x);
            if (x == n - one) return true;
        }
        return false;
    };
    if (!miller_rabin(two) || !miller_rabin(add(two, one))) return false;

    auto P = 3ULL;
    for (;; P++) {
        auto D = P * P - 4;
        auto jacobi_symbol = [&](auto n) {
            int j = 1;
            auto a = D;
            while (a) {
                for (; !(a & 1); a >>= 1)
                    if ((n & 7) == 3 || (n & 7) == 5) j = -j;
                if ((a & 3) == 3 && (n & 3) == 3) j = -j;

                swap(a, n);
                a %= n;
            }
            return n == 1 ? j : 0;
        };
        int j = jacobi_symbol(n);
        if (j == -1) break;
        if (!j && D % n) return false;
    }
    P = ((unsigned __int128) P * one % n);

    int s = countr_zero(n + 1);
    auto k = (n + 1) >> s, V_k = P, V_kp1 = sub(mul(P, P), two);
    for (int i = bit_width(k) - 2; i >= 0; i--) {
        auto V_2kp1 = sub(mul(V_k, V_kp1), P);

        if ((k >> i) & 1) {
            V_k = V_2kp1;
            V_kp1 = sub(mul(V_kp1, V_kp1), two);
        } else {
            V_kp1 = V_2kp1;
            V_k = sub(mul(V_k, V_k), two);
        }
    }
    if ((V_k == two && V_kp1 == P) || (V_k == sub(0, two) && V_kp1 == sub(0, P))) return true;

    while (--s) {
        if (!V_k) return true;
        V_k = sub(mul(V_k, V_k), two);
    }
    return false;
}

unsigned long long pollard_brent(unsigned long long n) {
    if (!(n & 1)) return 2;

    auto inv = n, one = -n % n;
    while (n * inv != 1) inv *= 2 - n * inv;

    auto add = [&](auto x, auto y) {
        return x >= n - y ? x - (n - y) : x + y;
    };

    auto mul = [&](auto x, auto y) -> unsigned long long {
        auto z = (unsigned __int128) x * y;
        unsigned long long q = z * inv;
        auto v = (z >> 64) + n - (((unsigned __int128) q * n) >> 64);
        return v >= n ? v - n : v;
    };

    static mt19937_64 rng(random_device{}());
    for (;;) {
        auto x = add(one, one), y = x, z = one, xs = x, d = 1ULL, c = rng() % (n - 1) + 1;

        auto f = [&](auto x) {
            return add(mul(x, x), c);
        };

        for (int i = 1; d == 1; i <<= 1, y = x) {
            for (int _ = 0; _ < i - 1; _++) x = f(x);
            for (int j = 0; j < i && d == 1; j += 300) {
                xs = x;
                for (int k = 0; k < min(300, i - j); k++) {
                    x = f(x);
                    z = mul(z, max(x, y) - min(x, y));
                }
                d = __gcd(z, n);
            }
        }

        if (d == n) d = 1;
        while (d == 1) {
            xs = f(xs);
            d = __gcd(max(xs, y) - min(xs, y), n);
        }
        if (d != n) return d;
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

        T d = pollard_brent(m);
        self(self, d);
        self(self, m / d);
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

istream & operator>>(istream &stream, __int128 &x) {
    string s;
    stream >> s;

    x = 0;
    for (int sgn = s[0] == '-' ? -1 : 1, i = sgn < 0; i < s.size(); i++) x = x * 10 + sgn * (s[i] - '0');
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
    if (accumulate(m.begin(), m.end(), (__int128) 0) != (__int128) 1 << n) impossible();

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
