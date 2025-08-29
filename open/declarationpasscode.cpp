#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    while (getline(cin, s)) {
        int n = stoi(s), d = s.size() * 2;

        double l = log10(n), r = log10(n + 1);
        l -= floor(l);
        r -= floor(r);
        if (l > r) swap(l, r);

        auto lg = log10(2);
        int p = ceil(d / lg);
        double f = fmod(p * lg, 1);
        auto update = [&]() {
            f += lg;
            if (f >= 1) f -= 1;
            p++;
        };
        while (floor(p * lg) + 1 <= d || !(l <= f && f <= r)) update();
        cout << p << "\n";
    }
}
