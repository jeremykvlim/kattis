#include <bits/stdc++.h>
using namespace std;

__int128 pow(__int128 base, long long exponent, long long mod = 1) {
    __int128 value = 1;
    for (; exponent; exponent >>= 1) {
        if (exponent & 1) value = (base * value) % mod;
        base = (base * base) % mod;
    }
    return value;
}

bool isprime(long long n) {
    if (n < 2) return false;

    for (int i = 2; i <= cbrt(n); i++)
        if (!(n % i)) return false;

    for (int i = 0; i < 1e5; i++) {
        auto b = rand() % (n - 1) + 1;
        if (__gcd(b, n) > 1 || pow(b, n - 1, n) > 1) return false;
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
