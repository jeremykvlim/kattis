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
        for (int i = 1; i <= m && total; i++) {
            int d = min(a[i], total);
            a[i] -= d;
            total -= d;
        }

        vector<int> parent_e(n + 1, 0), order{1};
        queue<int> q;
        q.emplace(1);
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (auto [u, i] : adj_list[v])
                if (!parent_e[u] && u != 1) {
                    parent_e[u] = i;
                    order.emplace_back(u);
                    q.emplace(u);
                }
        }

        vector<int> c(m + 1);
        vector<vector<int>> edge_indices(n + 1);
        vector<pair<int, int>> colors;
        for (int i = 1; i <= m; i++)
            if (a[i]) colors.emplace_back(a[i], i);
        sort(colors.rbegin(), colors.rend());
        reverse(order.begin(), order.end());
        for (int v : order)
            for (auto [u, i] : adj_list[v])
                if (parent_e[u] == i) {
                    edge_indices[u].emplace_back(i);
                    if (edge_indices[v].size() < edge_indices[u].size()) swap(edge_indices[v], edge_indices[u]);
                    edge_indices[v].insert(edge_indices[v].end(), edge_indices[u].begin(), edge_indices[u].end());
                    if (edge_indices[v].size() >= colors.back().first) {
                        auto [count, color] = colors.back();
                        colors.pop_back();
                        while (count--) {
                            c[edge_indices[v].back()] = color;
                            edge_indices[v].pop_back();
                        }
                        edge_indices[v].clear();
                    }
                    if (colors.empty()) goto done;
                }

        done:;
        cout << "Yes\n";
        for (int i = 1; i <= m; i++) cout << c[i] << " ";
        cout << "\n";
    }
}
