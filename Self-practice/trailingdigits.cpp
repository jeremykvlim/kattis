#include <bits/stdc++.h>
using namespace std;

pair<long long, long long> bezout(long long a, long long b) {
    if (!a) return {0, 1};
    auto [x, y] = bezout(b % a, a);
    return {y - (b / a) * x, x};
}

long long congruence(long long base, long long b, long long mod) {
    long long g = __gcd(base, mod);
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

    long long b;
    int d;
    string a;
    cin >> b >> d >> a;

    long long l = 0, r = sqrt(INT_MAX), m;
    while (l + 1 < r) {
        m = l + (r - l) / 2;

        long long base = 1, B = 0;
        for (int i = 0; i < m; i++) {
            base = base * 10 % b;
            B = (B * 10 + d) % b;
        }

        long long x = congruence(base, (b - B) % b, b);
        if (!x && !d) x = b / __gcd(base, b);

        bool valid = x != -1;
        if (!valid) {
            r = m;
            continue;
        }

        string s = to_string(x);
        if (!x) s.clear();
        s += string(m, '0' + d);
        if (s.size() == a.size()) {
            for (int i = 0; i < s.size(); i++)
                if (s[i] != a[i]) {
                    valid = s[i] < a[i];
                    break;
                }
        } else valid = s.size() < a.size();

        if (!valid) r = m;
        else l = m;
    }

    cout << l;
}
