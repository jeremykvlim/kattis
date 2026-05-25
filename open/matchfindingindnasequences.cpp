#include <bits/stdc++.h>
using namespace std;

struct HashedString {
    static inline unsigned long long B1 = 0, B2 = 0;
    static const unsigned long long MOD1 = 1e9 + 7, MOD2 = 1e9 + 9;

    int n;
    vector<unsigned long long> pref1, pref2;
    static inline vector<unsigned long long> p1{1}, p2{1};

    HashedString() : n(0), pref1(1, 0), pref2(1, 0) {}
    HashedString(const string &s) : n(s.size()), pref1(n + 1, 0), pref2(n + 1, 0) {
        if (!B1 && !B2) {
            mt19937_64 rng{random_device{}()};
            B1 = uniform_int_distribution(911382323ULL, MOD1 - 1)(rng);
            B2 = uniform_int_distribution(972663749ULL, MOD2 - 1)(rng);
        }
        while (p1.size() <= n || p2.size() <= n) {
            p1.emplace_back((p1.back() * B1) % MOD1);
            p2.emplace_back((p2.back() * B2) % MOD2);
        }
        for (int i = 0; i < n; i++) {
            auto v = (unsigned char) s[i] + 1;
            pref1[i + 1] = (pref1[i] * B1 + v) % MOD1;
            pref2[i + 1] = (pref2[i] * B2 + v) % MOD2;
        }
    }

    pair<unsigned long long, unsigned long long> pref_hash(int l, int r) const {
        auto h1 = (pref1[r] + MOD1 - (pref1[l] * p1[r - l]) % MOD1) % MOD1, h2 = (pref2[r] + MOD2 - (pref2[l] * p2[r - l]) % MOD2) % MOD2;
        return {h1, h2};
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string line;
    getline(cin, line);

    vector<string> markers;
    stringstream ss(line);
    while (getline(ss, line, ' ')) markers.emplace_back(line);

    string strand;
    getline(cin, strand);

    int n = strand.size(), m = markers.size(), total = 0;
    HashedString hs(strand);
    vector<vector<int>> pos(m);
    for (int i = 0; i < m; i++) {
        int len = markers[i].size();
        for (int j = 0; j + len <= n; j++)
            if (!strand.compare(j, len, markers[i])) pos[i].emplace_back(j);
        total += len;
    }

    vector<pair<int, int>> substrings;
    for (int j : pos[0]) {
        int l = j + markers[0].size(), limit = (n - j - total) / (m - 1);
        for (auto it = lower_bound(pos[1].begin(), pos[1].end(), l + 1); it != pos[1].end(); it++) {
            int r = *it, len = r - l;
            if (len > limit) break;

            auto h = hs.pref_hash(l, l + len);
            for (int i = 1; i < m - 1; i++) {
                r += markers[i].size();
                if (hs.pref_hash(r, r + len) != h || strand.compare(r + len, markers[i + 1].size(), markers[i + 1])) goto next;
                r += len;
            }
            substrings.emplace_back(l, len);
            next:;
        }
    }

    if (substrings.empty()) cout << "Failed to find equal substrings between the markers.";
    else
        for (auto [l, len] : substrings) cout << "Equal substrings found: " << strand.substr(l, len) << "\n";
}
