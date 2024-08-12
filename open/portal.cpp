#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    if (n <= 2) {
        cout << -1;
        exit(0);
    }

    auto abs = [](auto v) {
        auto mask = v >> 127;
        return (v + mask) ^ mask;
    };

    pair<__int128, __int128> a{1e7, 1e7}, b{1e7, 1e7}, c;
    vector<__int128> Y(n - 2);
    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;

        if (a.first == 1e7) {
            a = {x, y};
            continue;
        }

        auto adjust = [](auto &p) {
            if (p.first < 0) {
                p.first *= -1;
                p.second *= -1;
            }
        };

        if (b.first == 1e7) {
            b = {x - a.first, y - a.second};
            adjust(b);
            continue;
        }

        c = {x - a.first, y - a.second};
        adjust(c);

        if (b.first < c.first) swap(b, c);
        while (c.first) {
            auto q = b.first / c.first;
            b.first -= q * c.first;
            b.second -= q * c.second;
            if (b.first < c.first) swap(b, c);
        }

        Y[i - 2] = abs(c.second);
    }

    auto g = Y[0];
    for (int i = 1; i < n - 2; i++) g = __gcd(g, Y[i]);

    long long colours = abs(g * b.first);
    cout << (!colours ? -1 : colours);
}
