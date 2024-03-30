#include <bits/stdc++.h>
using namespace std;

pair<__int128, __int128> bezout(__int128 a, __int128 b) {
    if (!a) return {0, 1};
    auto [x, y] = bezout(b % a, a);
    return {y - (b / a) * x, x};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m;
    while (cin >> m && m) {
        string s;
        cin >> s;

        vector<__int128> pref(s.size() + 1, 0), inv(s.size() + 1, 1);
        __int128 base = 1, b = (long long) 1e16 + 61, mod = (1ULL << 63) + 27;
        for (int i = 0; i < s.size(); i++) {
            pref[i + 1] = (pref[i] + base * (s[i] - 'a' + 1)) % mod;
            base = (base * b) % mod;

            auto [x, y] = bezout(base, mod);
            if (x < 0) x += mod;
            inv[i + 1] = x;
        }

        auto pos = [&](int len) -> int {
            vector<vector<pair<long long, int>>> table(4e4);
            auto appearances = [&](long long k) -> int {
                int pos = k % table.size();
                for (auto &[key, count] : table[pos])
                    if (key == k) return ++count;

                table[pos].emplace_back(k, 1);
                return 1;
            };

            int pos = -1;
            for (int i = 0, j = i + len - 1; i < s.size() && j < s.size(); i++, j++) 
                if (appearances(((pref[j + 1] - pref[i] + mod) % mod) * inv[i] % mod) >= m) pos = i;

            return pos;
        };

        if (pos(1) == -1) {
            cout << "none\n";
            continue;
        }

        int l = 0, r = s.size() + 1, mid;
        while (l + 1 < r) {
            mid = l + (r - l) / 2;

            if (pos(mid) == -1) r = mid;
            else l = mid;
        }

        cout << l << " " << pos(l) << "\n";
    }
}
