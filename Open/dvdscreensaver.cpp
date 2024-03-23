#include <bits/stdc++.h>
using namespace std;

pair<long long, long long> bezout(long long a, long long b) {
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

    int T;
    cin >> T;

    while (T--) {
        long long ws, hs, wl, hl, xl, yl;
        cin >> ws >> hs >> wl >> hl >> xl >> yl;

        ws -= wl;
        hs -= hl;
        long long base = ws % hs, b = ((hs - yl) % hs - ((ws - xl) % ws) % hs + hs) % hs, t = congruence(base, b, hs);
        cout << ((t == -1) ? "Johnny will die waiting\n" : to_string(t * ws + (ws - xl) % ws) + "\n");
    }
}
