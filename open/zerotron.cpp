#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, s;
    cin >> n >> s;

    auto trailing_zeros = [&]() {
        int zeroes = 0;
        for (int p5 = 5; p5 <= n; p5 *= 5) zeroes += n / p5;
        return zeroes;
    };

    auto kamenetsky = [&]() -> int {
        if (n <= 3) return 1;
        return floor(n * log10(n / M_E) + log10(2 * M_PI * n) / 2) + 1;
    };

    int digits = kamenetsky(), z = trailing_zeros();
    if (s < z) cout << digits + s - z << " " << z;
    else cout << digits;
}
