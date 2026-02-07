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

    pair<unsigned long long, unsigned long long> split_pref_hash(int i) const {
        auto [ll, lr] = pref_hash(0, i);
        auto [rl, rr] = pref_hash(i + 1, n);
        return {(ll * p1[n - i - 1] + rl) % MOD1, (lr * p2[n - i - 1] + rr) % MOD2};
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int b, c;
    cin >> b >> c;

    vector<string> charms(c);
    vector<HashedString> hs(c);
    for (int i = 0; i < c; i++) {
        cin >> charms[i];

        hs[i] = charms[i];
    }

    vector<pair<int, vector<int>>> bugs(b);
    for (int i = 0; i < b; i++) {
        int k;
        cin >> k;

        bugs[i] = {i, vector<int>(k)};
        for (int &charm : bugs[i].second) cin >> charm;
    }

    auto cmp = [&](const auto &p1, const auto &p2) {
        auto charms1 = p1.second, charms2 = p2.second;
        for (int i = 0, j = 0, off1 = 0, off2 = 0;;) {
            if (i == charms1.size() && j == charms2.size()) return p1.first < p2.first;
            if (i == charms1.size()) return true;
            if (j == charms2.size()) return false;

            int c1 = charms1[i] - 1, c2 = charms2[j] - 1, len = min(charms[c1].size() - off1, charms[c2].size() - off2);
            auto h1 = hs[c1].pref_hash(off1, off1 + len), h2 = hs[c2].pref_hash(off2, off2 + len);
            if (h1 == h2) {
                off1 += len;
                off2 += len;
                if (off1 == charms[c1].size()) {
                    i++;
                    off1 = 0;
                }
                if (off2 == charms[c2].size()) {
                    j++;
                    off2 = 0;
                }
                continue;
            }

            int l = -1, r = len, m;
            while (l + 1 < r) {
                m = l + (r - l) / 2;

                if (hs[c1].pref_hash(off1, off1 + m + 1) == hs[c2].pref_hash(off2, off2 + m + 1)) l = m;
                else r = m;
            }
            return charms[c1][off1 + r] < charms[c2][off2 + r];
        }
    };
    sort(bugs.begin(), bugs.end(), cmp);
    for (auto [i, _] : bugs) cout << i + 1 << "\n";
}
