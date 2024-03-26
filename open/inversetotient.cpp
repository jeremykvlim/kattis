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
