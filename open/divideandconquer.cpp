#include <bits/stdc++.h>
using namespace std;

__int128 pow(long long base, long long exponent, long long mod = LLONG_MAX) {
    __int128 value = 1;
    for (; exponent; exponent >>= 1) {
        if (exponent & 1) value = (base * value) % mod;
        base = (base * base) % mod;
    }
    return value;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long b, d;
    cin >> b >> d;

    if (!(b % d)) {
        cout << "no";
        exit(0);
    }

    if (d == 2) {
        cout << "yes";
        exit(0);
    }

    auto order = d - 1;
    while (!(order & 1) && pow(b, order >> 1, d) == 1) order >>= 1;
    cout << (!(order & 1) ? "yes" : "no");
}
