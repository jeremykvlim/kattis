#include <bits/stdc++.h>
using namespace std;

struct Hash {
    template <typename T>
    static inline void combine(size_t &h, const T &v) {
        h ^= Hash{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }

    template <typename T>
    size_t operator()(const T &v) const {
        if constexpr (requires { tuple_size<T>::value; })
            return apply([](const auto &...e) {
                size_t h = 0;
                (combine(h, e), ...);
                return h;
            }, v);
        else if constexpr (requires { declval<T>().begin(); declval<T>().end(); } && !is_same_v<T, string>) {
            size_t h = 0;
            for (const auto &e : v) combine(h, e);
            return h;
        } else return hash<T>{}(v);
    }
};

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

    vector<string> dict(n);
    for (auto &s : dict) cin >> s;

    unordered_set<tuple<int, unsigned long long, unsigned long long>, Hash> hashes;
    vector<HashedString> words(n);
    for (int i = 0; i < n; i++) {
        words[i] = dict[i];
        auto [h1, h2] = words[i].pref_hash(0, dict[i].size());
        hashes.emplace(dict[i].size(), h1, h2);
    }

    vector<int> typos;
    for (int i = 0; i < n; i++) {
        int len = dict[i].size();
        if (len == 1) continue;

        for (int k = 0; k < len; k++) {
            auto [h1, h2] = words[i].split_pref_hash(k);

            if (hashes.count({len - 1, h1, h2})) {
                typos.emplace_back(i);
                break;
            }
        }
    }

    if (typos.empty()) cout << "NO TYPOS";
    else
        for (int i : typos) cout << dict[i] << "\n";
}
