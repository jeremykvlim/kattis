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

    int t;
    cin >> t;

    while (t--) {
        int k;
        string s;
        cin >> k >> s;

        int n = s.size();
        vector<vector<int>> next(n + 1, vector<int>(26, -1));
        vector<int> same(n + 1, 1);
        for (int i = n - 1; ~i; i--) {
            for (int j = 0; j < 26; j++) next[i][j] = next[i + 1][j];
            next[i][s[i] - 'a'] = i;
            if (i < n - 1 && s[i] == s[i + 1]) same[i] += same[i + 1];
        }

        HashedString hs(s);
        string str;
        int l = 0;
        for (char c = 'z'; c >= 'a' && l < n && k; c--) {
            while (l < n && s[l] == c) str += s[l++];

            vector<int> diff;
            for (int i = l + 1; i < n && ~next[i][c - 'a']; i++)
                if (s[i] == c && s[i - 1] != c) diff.emplace_back(i);
            if (diff.empty()) continue;
            sort(diff.begin(), diff.end(), [&](int i, int j) { return same[i] == same[j] ? i < j : same[j] < same[i]; });

            int i = 0, last = 0;
            for (; i < diff.size() && k; i++, k--) {
                int j = diff[i];

                last = j;
                for (; j < n && s[j] == c; j++) str += s[j];
                l = max(l, j);
            }

            int r = l;
            for (; i < diff.size(); i++) {
                int j = diff[i];

                auto greater = [&](int i, int j) {
                    if (i == j) return false;
                    auto lce = [&](int i, int j) {
                        int l = 0, r = min(n - i, n - j) + 1, m;
                        while (l + 1 < r) {
                            m = l + (r - l) / 2;

                            if (hs.pref_hash(i, i + m) == hs.pref_hash(j, j + m)) l = m;
                            else r = m;
                        }
                        return l;
                    };
                    int len = lce(i, j);
                    if (len == min(n - i, n - j)) return n - i > n - j;
                    return s[i + len] > s[j + len];
                };
                if (j > l && same[j] == same[last] && greater(j + same[j] - 1, r - 1)) r = j + same[j];
            }
            l = r;
        }

        str += s.substr(l);
        cout << str << '\n';
    }
}