#include <bits/stdc++.h>
using namespace std;

long long inverse(vector<long long> &pd, int i, long long n, long long curr, long long totient) {
    if (totient == 1) return min(n, curr);
    if (i < 0) return n;

    n = inverse(pd, i - 1, n, curr, totient);
    if (!(totient % (pd[i] - 1))) {
        totient /= pd[i] - 1;
        while (!(totient % pd[i])) totient /= pd[i];
        n = inverse(pd, i - 1, n, curr  / (pd[i] - 1) * pd[i], totient);
    }

    return n;
}

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

vector<long long> prime_divisors(long long n) {
    vector<long long> pd;
    for (auto i = 1LL; i <= sqrt(n); i++)
        if (!(n % i)) {
            if (isprime(i + 1)) pd.emplace_back(i + 1);
            if (n / i != i && isprime(n / i + 1)) pd.emplace_back(n / i + 1);
        }

    return pd;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long totient;
    cin >> totient;

    if (totient == 1) cout << 1;
    else if (totient & 1) cout << -1;
    else {
        auto pd = prime_divisors(totient);
        sort(pd.begin(), pd.end());

        auto n = inverse(pd, pd.size() - 1, LLONG_MAX, totient, totient);
        cout << (n == LLONG_MAX ? -1 : n);
    }
}
