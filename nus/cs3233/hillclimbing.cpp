#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int C;
    cin >> C;

    while (C--) {
        int n, m;
        cin >> n >> m;

        int total = 0;
        vector<int> a(m + 1);
        for (int i = 1; i <= m; i++) {
            cin >> a[i];
            total += a[i];
        }

        vector<vector<pair<int, int>>> adj_list(n + 1);
        for (int i = 1; i <= m; i++) {
            int u, v;
            cin >> u >> v;

            adj_list[u].emplace_back(v, i);
            adj_list[v].emplace_back(u, i);
        }

        if (total < (n + 1) / 2) {
            cout << "No\n";
            continue;
        }
        total -= (n + 1) / 2;

        if (total)
            for (int i = 1; i <= m && total; i++) {
                int d = min(a[i], total);
                a[i] -= d;
                total -= d;
            }

        vector<int> parent_e(n + 1, 0);
        vector<vector<int>> children(n + 1);
        vector<vector<pair<int, int>>> adj_list_tree(n + 1);
        queue<int> q;
        q.emplace(1);
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (auto [u, i] : adj_list[v])
                if (!parent_e[u] && u != 1) {
                    parent_e[u] = i;
                    children[v].emplace_back(u);
                    adj_list_tree[v].emplace_back(u, i);
                    adj_list_tree[u].emplace_back(v, i);
                    q.emplace(u);
                }
        }

        vector<pair<int, int>> colors;
        for (int i = 1; i <= m; i++)
            if (a[i]) colors.emplace_back(a[i], i);
        sort(colors.begin(), colors.end());

        vector<int> c(m + 1, 0);
        vector<bool> active_e(m + 1, false);
        for (int v = 2; v <= n; v++)
            if (parent_e[v]) active_e[parent_e[v]] = true;

        auto color_edge = [&](int i, int ci) {
            if (!i) return;
            c[i] = ci;
            active_e[i] = false;
        };

        auto dfs1 = [&](auto &&self, int v, int p, int &r, int ci) {
            if (r <= 0) return;
            for (auto [u, i] : adj_list_tree[v])
                if (u != p && active_e[i]) {
                    color_edge(i, ci);
                    r--;
                    self(self, u, v, r, ci);
                    if (r <= 0) return;
                }
        };

        int k = 0;
        vector<int> dp(n + 1, 0);
        auto dfs2 = [&](auto &&self, int v = 1) -> void {
            for (int u : children[v]) self(self, u);

            int sum = 0;
            multiset<pair<int, int>> active_v;
            for (int u : children[v]) {
                int i = parent_e[u];
                if (i && active_e[i]) {
                    active_v.emplace(dp[u] + 1, u);
                    sum += dp[u] + 1;
                }
            }

            for (; k < colors.size(); k++) {
                auto [remaining, ci] = colors[k];
                if (remaining > sum) break;

                auto color = [&](int u, int r) {
                    sum -= dp[u] + 1;
                    color_edge(parent_e[u], ci);
                    if (r) dfs1(dfs1, u, v, r, ci);
                };

                auto it = active_v.lower_bound({remaining, -1});
                if (it != active_v.end()) {
                    int u = it->second;
                    active_v.erase(it);
                    color(u, remaining - 1);
                } else {
                    int temp = remaining;
                    vector<int> remove;
                    for (it = active_v.begin(); it != active_v.end() && it->first < temp; it++) {
                        remove.emplace_back(it->second);
                        temp -= it->first;
                    }

                    for (int u : remove) {
                        it = active_v.find({dp[u] + 1, u});
                        active_v.erase(it);
                        color(u, dp[u]);
                    }

                    it = active_v.lower_bound({temp, -1});
                    int u = it->second;
                    active_v.erase(it);
                    color(u, temp - 1);
                }
            }

            for (int u : children[v]) {
                int i = parent_e[u];
                if (i && active_e[i]) dp[v] += dp[u] + 1;
            }
        };
        dfs2(dfs2);

        if (k < colors.size()) cout << "No\n";
        else {
            cout << "Yes\n";
            for (int i = 1; i <= m; i++) cout << c[i] << " ";
            cout << "\n";
        }
    }
}
