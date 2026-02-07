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

    int n;
    cin >> n;

    vector<HashedString> hs(n);
    for (auto &h : hs) {
        string s;
        cin >> s;

        h = s;
    }

    vector<tuple<pair<int, int>, pair<unsigned long long, unsigned long long>, int>> variants;
    for (int i = 0; i < n; i++) {
        int len = hs[i].n;
        for (int j = 0; j < len; j++) variants.emplace_back(make_pair(len, j), hs[i].split_pref_hash(j), i);
    }
    sort(variants.begin(), variants.end());

    vector<int> count(n, 0);
    for (int l = 0, r = 1; l < variants.size(); l = r++) {
        for (; r < variants.size() && get<0>(variants[l]) == get<0>(variants[r]) && get<1>(variants[l]) == get<1>(variants[r]); r++);

        if (l + 1 < r)
            for (int i = l; i < r; i++) count[get<2>(variants[i])] += r - l - 1;
    }
    for (int c : count) cout << c << "\n";
}
