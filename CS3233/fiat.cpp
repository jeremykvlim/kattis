#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 1e9 + 7;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<long long> fact(2 * n + 1, 1), a(n + 2, 1);
    for (int i = 1; i < n + 2; i++) {
        fact[i] = i * fact[i - 1] % MODULO;

        for (long long exponent = MODULO - 2, base = fact[i]; exponent; exponent >>= 1) {
            if (exponent & 1) a[i] = (base * a[i]) % MODULO;
            base = (base * base) % MODULO;
        }
    }

    for (int i = n + 2; i < fact.size(); i++) fact[i] = i * fact[i - 1] % MODULO;
    cout << fact[2 * n] * a[n] % MODULO * a[n + 1] % MODULO;
}
