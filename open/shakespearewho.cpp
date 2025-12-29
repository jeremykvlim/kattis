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
    HashedString(const string &s) : n((int)s.size()), pref1(n + 1, 0), pref2(n + 1, 0) {
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
            unsigned long long v = (unsigned long long)(unsigned char)s[i] + 1ULL;
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

    int longest = 0;
    vector<int> len(n);
    vector<string> words(n);
    for (int i = 0; i < n; i++) {
        cin >> words[i];

        longest = max(longest, len[i] = words[i].size());
    }

    vector<int> order(n);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int i, int j) { return len[i] < len[j]; });

    vector<HashedString> hs(n);
    for (int i : order) hs[i] = words[i];

    vector<vector<pair<int, int>>> adj_list(n);
    vector<int> degree(n, 0);
    vector<vector<bool>> visited(n, vector<bool>(n, false));
    for (int l = longest; l; l--) {
        unordered_map<pair<unsigned long long, unsigned long long>, vector<int>, Hash> indices;

        for (int i = 0; i < n; i++)
            if (len[i] >= l) indices[hs[i].pref_hash(0, l)].emplace_back(i);

        for (int i = 0; i < n; i++)
            if (len[i] >= l) {
                auto it = indices.find(hs[i].pref_hash(hs[i].n - l, hs[i].n));
                if (it != indices.end())
                    for (int j : it->second)
                        if (i != j && !visited[i][j]) {
                            visited[i][j] = true;
                            adj_list[i].emplace_back(j, l);
                            degree[j]++;
                        }
            }
    }

    queue<int> q;
    for (int i = 0; i < n; i++)
        if (!degree[i]) q.emplace(i);

    order.clear();
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        order.emplace_back(v);
        for (auto [u, w] : adj_list[v])
            if (!--degree[u]) q.emplace(u);
    }

    if (order.size() < n) {
        cout << "Shakespeare, who?";
        exit(0);
    }

    vector<int> dp(n, 0);
    for (int v : order)
        for (auto [u, w] : adj_list[v]) dp[u] = max(dp[u], dp[v] + w);
    cout << *max_element(dp.begin(), dp.end());
}
