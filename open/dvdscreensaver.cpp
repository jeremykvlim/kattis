#include <bits/stdc++.h>
using namespace std;

template <typename T>
pair<T, T> bezout(T a, T b) {
    if (!a) return {0, 1};
    auto [x, y] = bezout(b % a, a);
    return {y - (b / a) * x, x};
}

long long congruence(long long base, long long b, long long mod) {
    auto g = __gcd(base, mod);
    if (b % g) return -1;

    b /= g;
    base /= g;
    mod /= g;
    auto [x, y] = bezout(base, mod);
    x %= mod;
    if (x < 0) x += mod;
    return b * x % mod;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        long long ws, hs, wl, hl, xl, yl;
        cin >> ws >> hs >> wl >> hl >> xl >> yl;

        ws -= wl;
        hs -= hl;
        auto base = ws % hs, b = ((hs - yl) % hs - ((ws - xl) % ws) % hs + hs) % hs, x = congruence(base, b, hs);

        cout << ((x == -1) ? "Johnny will die waiting\n" : to_string(x * ws + (ws - xl) % ws) + "\n");
    }
}
