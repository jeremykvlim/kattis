#include <bits/stdc++.h>
using namespace std;

__int128 mul(__int128 x, __int128 y, long long mod) {
    auto product = x * y - mod * (__int128) (1.L / mod * x * y);
    return product + mod * (product < 0) - mod * (product >= mod);
}

__int128 pow(__int128 base, long long exponent, long long mod) {
    __int128 value = 1;
    for (; exponent; exponent >>= 1) {
        if (exponent & 1) value = mul(base, value, mod);
        base = mul(base, base, mod);
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
