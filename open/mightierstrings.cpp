#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;

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
            p1.emplace_back(p1.back() * B1 % MOD1);
            p2.emplace_back(p2.back() * B2 % MOD2);
        }
        for (int i = 0; i < n; i++) {
            auto v = (unsigned char)s[i] + 1;
            pref1[i + 1] = (pref1[i] * B1 + v) % MOD1;
            pref2[i + 1] = (pref2[i] * B2 + v) % MOD2;
        }
    }

    pair<unsigned long long, unsigned long long> pref_hash(int l, int r) const {
        auto h1 = (pref1[r] + MOD1 - pref1[l] * p1[r - l] % MOD1) % MOD1;
        auto h2 = (pref2[r] + MOD2 - pref2[l] * p2[r - l] % MOD2) % MOD2;
        return {h1, h2};
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    string t;
    vector<int> offset(n), len(n);
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;

        offset[i] = t.size();
        len[i] = s.size();
        t += s;
    }

    int limit = 0;
    for (; (limit + 1) * (limit + 2) * (limit + 3) / 6 <= t.size() && (limit + 1) * (limit + 2) / 2 <= n; limit++);

    HashedString hs(t);
    gp_hash_table<tuple<unsigned long long, unsigned long long, int>, int, Hash> indices;
    vector<int> start(1), length(1), freq(1);
    for (int i = 0; i < n; i++) {
        if (len[i] > limit) continue;

        auto [h1, h2] = hs.pref_hash(offset[i], offset[i] + len[i]);
        auto it = indices.find({h1, h2, len[i]});
        if (it != indices.end()) freq[it->second]++;
        else {
            start.emplace_back(offset[i]);
            length.emplace_back(len[i]);
            freq.emplace_back(1);
            indices.insert({{h1, h2, len[i]}, (int) freq.size() - 1});
        }
    }

    int m = freq.size();
    vector<vector<int>> adj_list(m);
    for (int u = 1; u < m; u++) {
        int v = 0;
        if (length[u] > 1) {
            auto [h1, h2] = hs.pref_hash(start[u], start[u] + length[u] - 1);
            auto it = indices.find({h1, h2, length[u] - 1});
            if (it == indices.end()) continue;
            v = it->second;
        }
        adj_list[v].emplace_back(u);
    }

    int total = 0;
    vector<int> count(m, 0);
    stack<int> undo;
    auto dfs = [&](auto &&self, int v) -> void {
        bool mighty = true;
        int l = start[v], r = l + length[v], version = undo.size();
        for (int i = r - 1; i >= l; i--) {
            auto [h1, h2] = hs.pref_hash(i, r);
            auto it = indices.find({h1, h2, r - i});
            if (it == indices.end()) {
                mighty = false;
                break;
            }

            int u = it->second;
            count[u]++;
            undo.emplace(u);
            if (count[u] > freq[u]) {
                mighty = false;
                break;
            }
        }

        if (mighty) {
            total += freq[v];
            for (int u : adj_list[v]) self(self, u);
        }

        while (undo.size() > version) {
            count[undo.top()]--;
            undo.pop();
        }
    };
    for (int v : adj_list[0]) dfs(dfs, v);
    cout << total;
}
