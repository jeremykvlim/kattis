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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<vector<int>> onion_primes(11);
    for (int b = 2; b <= 10; b++) {
        int pow = 0;
        for (auto val = 1LL; val <= INT_MAX; val *= b) pow++;

        vector<long long> pb(pow + 1, 1);
        for (int i = 1; i <= pow; i++) pb[i] = pb[i - 1] * b;

        vector<pair<int, int>> curr;
        for (int p = 2; p < b; p == 2 ? p++ : p += 2)
            if (isprime(p)) {
                curr.emplace_back(p, 1);
                onion_primes[b].emplace_back(p);
            }

        for (int p = b; p < pb[2]; p++)
            if (isprime(p)) {
                curr.emplace_back(p, 2);
                onion_primes[b].emplace_back(p);
            }

        while (!curr.empty()) {
            vector<pair<int, int>> temp;
            for (auto [p, len] : curr) {
                if (pow < len + 2) continue;

                for (int d_first = 1; d_first < b; d_first++) {
                    auto v = pb[len + 1] * d_first + (long long) p * b;
                    for (int d_last = 0; d_last < b; d_last++) {
                        auto q = v + d_last;
                        if (q > INT_MAX) continue;

                        if (isprime(q)) {
                            temp.emplace_back(q, len + 2);
                            onion_primes[b].emplace_back(q);
                        }
                    }
                }
            }
            curr = temp;
        }

        sort(onion_primes[b].begin(), onion_primes[b].end());
        onion_primes[b].erase(unique(onion_primes[b].begin(), onion_primes[b].end()), onion_primes[b].end());
    }

    int t;
    cin >> t;

    while (t--) {
        int b, A, B;
        cin >> b >> A >> B;

        auto l = lower_bound(onion_primes[b].begin(), onion_primes[b].end(), A), r = upper_bound(onion_primes[b].begin(), onion_primes[b].end(), B);
        cout << r - l << "\n";
    }
}
