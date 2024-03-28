#include <bits/stdc++.h>
using namespace std;

__int128 pow(__int128 base, long long exponent, long long mod = LLONG_MAX) {
    __int128 value = 1;
    for (; exponent; exponent >>= 1) {
        if (exponent & 1) value = (base * value) % mod;
        base = (base * base) % mod;
    }
    return value;
}

bool isprime(long long n) {
    if (n < 2) return false;

    mt19937_64 rng(random_device{}());
    for (int i = 0; i < 3; i++) {
        auto temp = n - 1;
        auto b = rng() % temp + 1;
        if (pow(b, temp, n) != 1) return false;

        while (!(temp & 1)) temp >>= 1;

        auto p = pow(b, temp, n);
        while (1 < p && p < n - 1) {
            p = (p * p) % n;
            temp <<= 1;
        }

        if (p == 1 && !(temp & 1)) return false;
    }

    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    cin >> n;

    cout << (isprime(n) ? "YES" : "NO");
}
