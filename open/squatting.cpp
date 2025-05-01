#include <bits/stdc++.h>
using namespace std;

template <unsigned long long B1 = 0x9e3779b97f4a7c15, unsigned long long B2 = 0xbf58476d1ce4e5b9>
struct HashedString {
    int n;
    static const unsigned long long b1 = B1, b2 = B2;
    static inline vector<unsigned long long> p1, p2;
    vector<unsigned long long> pref1, pref2, suff1, suff2;

    HashedString() {};
    HashedString(const string &s) : n(s.size()), pref1(s.size() + 1, 0), pref2(s.size() + 1, 0),
                                                 suff1(s.size() + 1, 0), suff2(s.size() + 1, 0) {
        if (p1.size() != n + 1) {
            p1.resize(n + 1);
            p2.resize(n + 1);
            p1[0] = p2[0] = 1;
            for (int i = 0; i < n; i++) {
                p1[i + 1] = p1[i] * b1;
                p2[i + 1] = p2[i] * b2;
            }
        }

        for (int i = 0; i < n; i++) {
            pref1[i + 1] = pref1[i] * b1 + (unsigned char) s[i];
            pref2[i + 1] = pref2[i] * b2 + (unsigned char) s[i];
            suff1[i + 1] = suff1[i] * b1 + (unsigned char) s[n - 1 - i];
            suff2[i + 1] = suff2[i] * b2 + (unsigned char) s[n - 1 - i];
        }
    }

    pair<unsigned long long, unsigned long long> pref_hash(int l, int r) const {
        auto h1 = pref1[r] - pref1[l] * p1[r - l], h2 = pref2[r] - pref2[l] * p2[r - l];
        return {h1, h2};
    }

    pair<unsigned long long, unsigned long long> suff_hash(int l, int r) const {
        auto h1 = suff1[n - l] - suff1[n - r] * p1[r - l], h2 = suff2[n - l] - suff2[n - r] * p2[r - l];
        return {h1, h2};
    }

    pair<unsigned long long, unsigned long long> split_pref_hash(int i) const {
        auto [ll, lr] = pref_hash(0, i);
        auto [rl, rr] = pref_hash(i + 1, n);

        auto h1 = ll * p1[n - i - 1] + rl, h2 = lr * p2[n - i - 1] + rr;
        return {h1, h2};
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
