#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 998244353;

long long pow(long long base, long long exponent, long long mod = LLONG_MAX) {
    auto value = 1LL;
    for (; exponent; exponent >>= 1) {
        if (exponent & 1) value = (base * value) % mod;
        base = (base * base) % mod;
    }
    return value;
}

vector<int> sieve(int n) {
    vector<int> mobius(n + 1, 1), lpf(n + 1, 0);
    for (int i = 2; i <= n; i++)
        if (!lpf[i]) {
            for (int j = i; j <= n; j += i) {
                lpf[j] = i;
                mobius[j] *= -1;
            }

            for (auto j = (long long) i * i; j <= n; j += i * i) mobius[j] = 0;
        }

    return mobius;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m, k;
    cin >> m >> k;

    auto mobius = sieve(k);
    auto bags = 1LL;
    for (int i = 1; i <= k; i++) bags = (bags + MODULO + mobius[i] * (pow(k / i * 2 + 1, m, MODULO) - 1)) % MODULO;
    cout << bags;
}
