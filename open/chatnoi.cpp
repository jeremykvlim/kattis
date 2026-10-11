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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<string> words;
    unordered_map<string, int> compress;
    vector<int> indices(n);
    for (int &i : indices) {
        string s;
        cin >> s;

        auto [it, inserted] = compress.try_emplace(s, words.size());
        if (inserted) words.emplace_back(s);
        i = it->second;
    }

    array<int, 10> seq{};
    for (int i = 0; i < k; i++) seq[i] = indices[i];

    unordered_map<array<int, 10>, int, Hash> seq_node;
    vector<int> nodes(n - k + 1), seq_last;
    for (int i = 0; i <= n - k; i++) {
        auto [it, inserted] = seq_node.try_emplace(seq, seq_last.size());
        if (inserted) seq_last.emplace_back(indices[i + k - 1]);
        nodes[i] = it->second;

        if (i < n - k) {
            for (int j = 0; j < k - 1; j++) seq[j] = seq[j + 1];
            seq[k - 1] = indices[i + k];
        }
    }

    unordered_map<pair<int, int>, int, Hash> freq;
    for (int i = 0; i < n - k; i++) freq[{nodes[i], nodes[i + 1]}]++;

    vector<array<int, 3>> edges;
    for (auto [uv, f] : freq) edges.push_back({uv.first, uv.second, f});
    sort(edges.begin(), edges.end(), [&](auto &e1, auto &e2) { return e1[2] > e2[2]; });

    int V = seq_last.size();
    vector<int> seen(V, -1), degree(V), longest(V), next(V), active;
    vector<vector<pair<int, int>>> adj_list(V), dp(V);
    for (int i = 0; i < edges.size(); i++) adj_list[edges[i][1]].emplace_back(edges[i][0], i);
    for (int e = edges.size(); e;) {
        int f = edges[e - 1][2];

        active.clear();
        for (int i = 0; i < e; i++) {
            auto [u, v, _] = edges[i];
            for (int t : {u, v})
                if (seen[t] != e) {
                    seen[t] = e;
                    degree[t] = longest[t] = 0;
                    next[t] = -1;
                    active.emplace_back(t);
                }
            degree[u]++;
        }

        queue<int> q;
        for (int v : active)
            if (!degree[v]) q.emplace(v);

        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (auto [u, i] : adj_list[v]) {
                if (i >= e) break;
                if (longest[u] < longest[v] + 1) {
                    longest[u] = longest[v] + 1;
                    next[u] = i;
                }
                if (!--degree[u]) q.emplace(u);
            }
        }

        for (int i = 0; i < e; i++) {
            auto [u, v, _] = edges[i];
            if (degree[v]) {
                longest[u] = INT_MAX;
                next[u] = i;
            }
        }

        for (int v : active)
            if (~next[v]) dp[v].emplace_back(longest[v], next[v]);

        for (; e && edges[e - 1][2] == f; e--);
    }

    int q;
    cin >> q;

    while (q--) {
        int m;
        cin >> m;

        array<int, 10> query{};
        for (int i = 0; i < k; i++) {
            string s;
            cin >> s;

            query[i] = compress.find(s)->second;
        }

        auto it = seq_node.find(query);
        int v = -1, l = -1;
        if (it != seq_node.end()) {
            v = it->second;
            int r = dp[v].size(), mid;
            while (l + 1 < r) {
                mid = l + (r - l) / 2;

                if (dp[v][mid].first >= m) l = mid;
                else r = mid;
            }
        }

        while (m--) cout << (words[!~l ? 0 : seq_last[v = edges[dp[v][l].second][1]]]) << " ";
        cout << "\n";
    }
}