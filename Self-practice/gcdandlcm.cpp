#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long x, y;
    cin >> x >> y;

    if (x == 1 && y == 1) {
        cout << "1 1";
        exit(0);
    }

    if (x > y) swap(x, y);
    if (y % x) exit(0);
    auto z = y / x;

    vector<pair<long long, long long>> pairs;
    for (auto i = 1LL; i * i <= z; i++)
        if (!(z % i)) {
            auto p = i, q = z / i;
            if (__gcd(p, q) == 1) pairs.emplace_back(p * x, q * x);
        }

    for (auto [a, b] : pairs) {
        cout << a << " " << b << "\n";
        if (a == b) exit(0);
    }
    reverse(pairs.begin(), pairs.end());
    for (auto [a, b] : pairs) cout << b << " " << a << "\n";
}
