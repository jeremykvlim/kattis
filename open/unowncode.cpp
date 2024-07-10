#include <bits/stdc++.h>
using namespace std;

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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> a(n), floor(n, 1);
    for (int i = 0; i < n; i++) {
        cin >> a[i];

        while (floor[i] <= a[i]) floor[i] *= 10;
    }

    vector<int> codes;
    for (int p2 = 1; p2 <= 1e4; p2 *= 2)
        for (int p5 = 1; p2 * p5 <= 1e8; p5 *= 5) {
            if (p2 * p5 > 1) codes.emplace_back(p2 * p5);
            codes.emplace_back(p2 * p5 + 1);
        }
    sort(codes.begin(), codes.end());
    codes.erase(unique(codes.begin(), codes.end()), codes.end());

    for (int c : codes) {
        for (int i = 0; i < n; i++)
            if (pow(a[i], c, floor[i]) != a[i]) goto next;

        cout << c;
        exit(0);

        next:;
    }

    cout << -1;
}
