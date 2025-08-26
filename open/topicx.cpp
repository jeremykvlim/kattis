#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    string s, x;
    cin >> t >> s >> x;

    vector<int> m(t);
    vector<string> topics(t);
    vector<vector<string>> adj_list_str(t);
    unordered_map<string, int> indices;
    for (int i = 0; i < t; i++) {
        int n;
        cin >> topics[i] >> m[i] >> n;

        indices[topics[i]] = i;
        adj_list_str[i].resize(n);
        for (auto &topic : adj_list_str[i]) cin >> topic;
    }

    vector<bool> avoid(t, false);
    vector<vector<int>> adj_list(t);
    for (int i = 0; i < t; i++)
        for (auto &topic : adj_list_str[i]) {
            if (topic == x) avoid[i] = true;
            else adj_list[i].emplace_back(indices[topic]);
        }

    int src = indices[s];
    vector<int> state(t, 0);
    auto dfs1 = [&](auto &&self, int v) -> bool {
        state[v] = 1;
        for (auto u : adj_list[v])
            if (!state[u]) {
                if (self(self, u)) return true;
            } else if (state[u] == 1) return true;

        state[v] = 2;
        return false;
    };
    if (dfs1(dfs1, src)) {
        cout << "SAFE";
        exit(0);
    }

    vector<int> dp(t, INT_MIN);
    auto dfs2 = [&](auto &&self, int v) {
        if (dp[v] != INT_MIN) return dp[v];
        if (avoid[v]) dp[v] = 0;
        for (int u : adj_list[v]) dp[v] = max(dp[v], self(self, u));
        return dp[v] += m[v];
    };
    cout << dfs2(dfs2, src);
}
