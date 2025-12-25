#include <bits/stdc++.h>
using namespace std;

template <unsigned long long B1 = 0x9e3779b97f4a7c15, unsigned long long B2 = 0xbf58476d1ce4e5b9>
struct HashedString {
    int n;
    vector<unsigned long long> pref1, pref2;
    static inline vector<unsigned long long> p1{1}, p2{1};

    HashedString() : n(0), pref1(1, 0), pref2(1, 0) {}
    HashedString(const string &s) : n(s.size()), pref1(s.size() + 1, 0), pref2(s.size() + 1, 0) {
        while (p1.size() <= n || p2.size() <= n) {
            p1.emplace_back(p1.back() * B1);
            p2.emplace_back(p2.back() * B2);
        }

        for (int i = 0; i < n; i++) {
            pref1[i + 1] = pref1[i] * B1 + (unsigned char) s[i];
            pref2[i + 1] = pref2[i] * B2 + (unsigned char) s[i];
        }
    }

    pair<unsigned long long, unsigned long long> pref_hash(int l, int r) const {
        auto h1 = pref1[r] - pref1[l] * p1[r - l], h2 = pref2[r] - pref2[l] * p2[r - l];
        return {h1, h2};
    }

    pair<unsigned long long, unsigned long long> split_pref_hash(int i) const {
        auto [ll, lr] = pref_hash(0, i);
        auto [rl, rr] = pref_hash(i + 1, n);
        return {ll * p1[n - i - 1] + rl, lr * p2[n - i - 1] + rr};
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<HashedString<>> hs(n);
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
