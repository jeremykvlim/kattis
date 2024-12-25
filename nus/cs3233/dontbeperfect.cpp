#include <bits/stdc++.h>
using namespace std;

struct Hash {
    size_t operator()(const vector<int> &v) const {
        auto h = 0ULL;
        for (int e : v) h ^= e + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    if (n & 1) {
        cout << 1;
        exit(0);
    }

    vector<vector<int>> adj_list(n + 1);
    for (int i = 2; i <= n; i++) {
        int p;
        cin >> p;

        adj_list[p].emplace_back(i);
    }

    vector<int> subtree_size(n + 1, 1);
    int even = 0;
    auto dfs1 = [&](auto &&self, int v = 1, int prev = -1, int depth = 0) -> void {
        for (int u : adj_list[v])
            if (u != prev) {
                self(self, u, v, depth + 1);
                subtree_size[v] += subtree_size[u];
            }

        even += !(subtree_size[v] & 1);
    };
    dfs1(dfs1);

    if (even == n / 2) {
        cout << 0;
        exit(0);
    }

    vector<vector<pair<pair<int, int>, vector<int>>>> dp(n + 1);
    auto dfs2 = [&](auto &&self, int v = 1) -> void {
        for (int u : adj_list[v]) self(self, u);

        vector<pair<pair<int, int>, vector<int>>> added{{{1, 0}, {}}}, unadded{{{0, 1}, {}}};
        for (int u : adj_list[v]) {
            vector<pair<pair<int, int>, vector<int>>> temp1, temp2;
            for (auto [p1, cliques1] : added)
                for (auto [p2, cliques2] : dp[u]) {
                    vector<int> cliques;
                    for (int size : cliques1) cliques.emplace_back(size);
                    for (int size : cliques2) cliques.emplace_back(size);

                    auto [S_size1, add1] = p1;
                    auto [S_size2, add2] = p2;
                    if (add2) cliques.emplace_back(add2);
                    temp1.emplace_back(make_pair(S_size1 + S_size2, 0), cliques);
                }

            for (auto [p1, cliques1] : unadded)
                for (auto [p2, cliques2] : dp[u]) {
                    vector<int> cliques;
                    for (int size : cliques1) cliques.emplace_back(size);
                    for (int size : cliques2) cliques.emplace_back(size);

                    auto [S_size1, add1] = p1;
                    auto [S_size2, add2] = p2;
                    temp2.emplace_back(make_pair(S_size1 + S_size2, add1 + add2), cliques);
                }

            added = temp1;
            unadded = temp2;
        }
        for (auto config : added) {
            sort(config.second.begin(), config.second.end());
            dp[v].emplace_back(config);
        }
        for (auto config : unadded) {
            sort(config.second.begin(), config.second.end());
            dp[v].emplace_back(config);
        }
        sort(dp[v].begin(), dp[v].end());
        dp[v].erase(unique(dp[v].begin(), dp[v].end()), dp[v].end());
    };
    dfs2(dfs2);

    int result = 0;
    vector<unordered_set<vector<int>, Hash>> visited(n + 1);
    for (auto [p, cliques] : dp[1]) {
        auto [S_size, add] = p;
        if (!S_size) continue;

        auto dfs3 = [&](auto &&self, vector<int> cliques) {
            sort(cliques.begin(), cliques.end());
            if (visited[S_size].count(cliques)) return;
            visited[S_size].emplace(cliques);

            for (int &size : cliques)
                if (!(size & 1)) {
                    swap(size, cliques.back());

                    int s = cliques.back();
                    cliques.pop_back();

                    for (int j = 0; j < cliques.size(); j++) {
                        cliques[j] += s;
                        self(self, cliques);
                        cliques[j] -= s;
                    }
                    return;
                }

            int odd = cliques.size();
            if (odd == S_size + 2) {
                result++;
                return;
            }

            for (int i = 0; i < odd; i++)
                for (int j = 0; j < i; j++) {
                    vector<int> merged;
                    for (int k = 0; k < odd; k++)
                        if (i != k && j != k) merged.emplace_back(cliques[k]);
                    merged.emplace_back(cliques[i] + cliques[j]);

                    self(self, merged);
                }
        };

        if (add) cliques.emplace_back(add);
        dfs3(dfs3, cliques);
    }
    cout << result;
}
