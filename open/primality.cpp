#include <bits/stdc++.h>
using namespace std;

long long mul(long long x, long long y, long long mod) {
    auto product = x * y - mod * (long long) (1.L / mod * x * y);
    return product + mod * (product < 0) - mod * (product >= mod);
}

long long pow(long long base, long long exponent, long long mod) {
    auto value = 1LL;
    for (; exponent; exponent >>= 1) {
        if (exponent & 1) value = mul(value, base, mod);
        base = mul(base, base, mod);
    }

    return value;
}

bool isprime(long long n) {
    if (n < 2) return false;
    if (n % 6 % 4 != 1) return (n | 1) == 3;

    vector<int> bases{2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    int s = __builtin_ctzll(n - 1);
    auto d = n >> s;
    for (int a : bases) {
        auto p = pow(a % n, d, n);
        int i = s;
        while (1 < p && p < n - 1 && a % n && i--) p = mul(p, p, n);
        if (p != n - 1 && i != s) return false;
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
