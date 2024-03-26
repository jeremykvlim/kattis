#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 1e9 + 7;

long long pow(long long base, long long exponent, long long mod = 1) {
    auto value = 1LL;
    for (; exponent; exponent >>= 1) {
        if (exponent & 1) value = (base * value) % mod;
        base = (base * base) % mod;
    }
    return value;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<long long> fact(2 * n + 1, 1), a(n + 2, 1);
    for (int i = 1; i < n + 2; i++) {
        fact[i] = i * fact[i - 1] % MODULO;

        a[i] *= pow(fact[i], MODULO - 2, MODULO);
    }

    for (int i = n + 2; i < fact.size(); i++) fact[i] = i * fact[i - 1] % MODULO;
    cout << fact[2 * n] * a[n] % MODULO * a[n + 1] % MODULO;
}
