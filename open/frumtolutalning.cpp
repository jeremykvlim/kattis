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

long long pi(long long n) {
    if (n <= 1) return 0;
    if (n == 2) return 1;

    int sqrtn = sqrt(n), size = (sqrtn + 1) / 2;
    vector<int> small(size), approx(size);
    vector<long long> large(size);
    iota(small.begin(), small.end(), 0);
    for (int i = 0; i < size; i++) {
        approx[i] = 2 * i + 1;
        large[i] = (n / approx[i] - 1) / 2;
    }

    auto div = [](double n, long long d) -> int {
        return n / d;
    };

    auto half = [](int n) {
        return (n - 1) / 2;
    };

    vector<bool> prime(sqrtn + 1, true);
    int count = 0;
    for (int p = 3; p <= sqrtn; p += 2)
        if (prime[p]) {
            int sq = p * p;
            if ((long long) sq * sq > n) break;

            prime[p] = false;
            for (int i = sq; i <= sqrtn; i += 2 * p) prime[i] = false;

            int s = 0;
            for (int i = 0; i < size; i++) {
                int q = approx[i];
                if (!prime[q]) continue;

                auto semiprime = (long long) p * q;
                large[s] = large[i] + count;
                large[s] -= semiprime <= sqrtn ? large[small[semiprime / 2] - count] : small[half(div(n, semiprime))];
                approx[s++] = q;
            }
            size = s;

            for (int i = half(sqrtn), j = (sqrtn / p - 1) | 1; j >= p; j -= 2)
                for (int k = small[j / 2] - count; i >= j * p / 2; i--) small[i] -= k;

            count++;
        }

    for (int i = 1; i < size; i++) large[0] -= large[i];
    large[0] += (long long) (size + 2 * (count - 1)) * (size - 1) / 2;
    for (int i = 1; i < size; i++) {
        int q = approx[i];
        long long m = n / q;

        int k = small[half(m / q)] - count;
        if (k <= i) break;

        for (int j = i + 1; j <= k; j++) large[0] += small[half(div(m, approx[j]))];
        large[0] -= (long long) (k - i) * (count + i - 1);
    }

    return large[0] + 1;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long a, b;
    cin >> a >> b;

    if (b - a <= 1e5) {
        int count = a < 3;
        for (auto c = a + !(a & 1); c <= b; c += 2) count += isprime(c);
        cout << count;
    } else if (b - a <= 1e7) {
        auto c = a + !(a & 1);
        auto primes = sieve(sqrtl(b));
        primes.erase(primes.begin());
        vector<bool> prime((b - c) / 2 + 1, true);
        prime[0] = c != 1;
        for (int p : primes) {
            auto pp = (long long) p * p;
            if (pp > b) break;
            if (pp < a) pp = a + ((p - (a % p)) % p);
            if (!(pp & 1)) pp += p;
            for (; pp <= b; pp += 2 * p) prime[(pp - c) / 2] = false;
        }
        cout << (a < 3) + count(prime.begin(), prime.end(), true);
    } else cout << pi(b) - pi(a - 1);
}
