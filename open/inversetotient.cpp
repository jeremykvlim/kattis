#include <bits/stdc++.h>
using namespace std;

long long phi_inv(vector<long long> &pd, int i, long long n, long long curr, long long totient) {
    if (totient == 1) return min(n, curr);
    if (i < 0) return n;

    n = phi_inv(pd, i - 1, n, curr, totient);
    if (!(totient % (pd[i] - 1))) {
        totient /= pd[i] - 1;
        while (!(totient % pd[i])) totient /= pd[i];
        n = phi_inv(pd, i - 1, n, curr  / (pd[i] - 1) * pd[i], totient);
    }

    return n;
}

template <typename T>
T mul(T x, T y, T mod) {
    long long px = x, py = y, pmod = mod;
    auto product = px * py - pmod * (long long) (1.L / pmod * px * py);
    return product + pmod * (product < 0) - pmod * (product >= pmod);
}

template <typename T>
T pow(T base, T exponent, T mod) {
    T value = 1;
    while (exponent) {
        if (exponent & 1) value = mul(value, base, mod);
        base = mul(base, base, mod);
        exponent >>= 1;
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
    for (int i = 1; i <= sqrt(n); i++)
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

        auto n = phi_inv(pd, pd.size() - 1, LLONG_MAX, totient, totient);
        cout << (n == LLONG_MAX ? -1 : n);
    }
}
