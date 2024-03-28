#include <bits/stdc++.h>
using namespace std;

long long mul(long long x, long long y, long long mod) {
    long long product = (__int128) x * y - mod * (long long) (1.L / mod * x * y);
    return product + mod * (product < 0) - mod * (product >= mod);
}

long long pow(long long base, long long exponent, long long mod) {
    auto value = 1LL;
    for (; exponent; exponent >>= 1) {
        if (exponent & 1) value = mul(value, base, mod);
        base = mul(base, base, mod);
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

    set<long long> codes;
    for (auto p2 = 1LL; p2 <= 1e4; p2 *= 2)
        for (auto p5 = 1LL; p2 * p5 <= 1e8; p5 *= 5) {
            if (p2 * p5 > 1) codes.emplace(p2 * p5);
            codes.emplace(p2 * p5 + 1);
        }

    for (auto c : codes) {
        for (int i = 0; i < n; i++)
            if (pow(a[i], c, floor[i]) != a[i]) goto next;

        cout << c;
        exit(0);

        next:;
    }

    cout << -1;
}
