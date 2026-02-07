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

    string x, y;
    cin >> x >> y;

    auto [it_x, it_y] = mismatch(x.rbegin(), x.rend(), y.rbegin(), y.rend());
    x.erase(it_x.base(), x.end());
    y.erase(it_y.base(), y.end());

    if (x.empty() && y.empty()) {
        cout << 1;
        exit(0);
    }

    int n = x.size();
    auto s = x + "#" + y;
    HashedString hs(s);

    auto equal = [&](int l1, int r1, int l2, int r2) -> bool {
        int len1 = r1 - l1 + 1, len2 = r2 - l2 + 1;
        if (len1 != len2) return false;
        return hs.pref_hash(l1, r1 + 1) == hs.pref_hash(l2, r2 + 1);
    };

    vector<int> indices1, indices2;
    for (int i = 0; i < n; i++) {
        if (equal(0, i, n + n - i, n + n)) indices1.emplace_back(i);
        if (equal(n - 1 - i, n - 1, n + 1, n + 1 + i)) indices2.emplace_back(i);
    }
    
    reverse(indices1.begin(), indices1.end());
    reverse(indices2.begin(), indices2.end());
    if (indices1.size() > 2) indices1.resize(2);
    if (indices2.size() > 2) indices2.resize(2);

    for (int i : indices1)
        for (int j : indices2) {
            if (i > n - j - 2) continue;
            if (i == n - j - 2 || equal(i + 1, n - j - 2, n + j + 2, n + n - i - 1)) {
                cout << 1;
                exit(0);
            }
        }
    cout << 0;
}
