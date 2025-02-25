#include <bits/stdc++.h>
using namespace std;

template <typename T, typename U, typename V>
T mul(U x, V y, T mod) {
    return ((unsigned __int128) x * y) % mod;
}

template <typename T>
T pow(T base, T exponent, T mod) {
    T value = 1;
    while (exponent) {
        if (exponent & 1) value = mul(base, value, mod);
        base = mul(base, base, mod);
        exponent >>= 1;
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
    while (!(order & 1) && pow(b % d, order >> 1, d) == 1) order >>= 1;

    cout << (!(order & 1) ? "yes" : "no");
}
