#include <bits/stdc++.h>
using namespace std;

template <typename T>
pair<T, T> bezout(T a, T b) {
    if (!a) return {0, 1};
    auto [x, y] = bezout(b % a, a);
    return {y - (b / a) * x, x};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    int q;
    cin >> s >> q;

    vector<__int128> pref(s.size() + 1, 0), inv(s.size() + 1, 1);
    __int128 base = 1, b = (long long) 1e16 + 61, mod = (1ULL << 63) + 27;
    for (int i = 0; i < s.size(); i++) {
        pref[i + 1] = (pref[i] + base * (s[i] - 'a' + 1)) % mod;
        base = (base * b) % mod;

        auto [x, y] = bezout(base, mod);
        if (x < 0) x += mod;
        inv[i + 1] = x;
    }

    while (q--) {
        int l, r;
        cin >> l >> r;
        cout << (long long) (((pref[r] - pref[l] + mod) % mod) * inv[l] % mod) << "\n";
    }
}
