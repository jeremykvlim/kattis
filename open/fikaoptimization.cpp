#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj_list(n + 1);
    for (int _ = 0; _ < n - 1; _++) {
        int u, v;
        cin >> u >> v;

        adj_list[u].emplace_back(v);
        adj_list[v].emplace_back(u);
    }

    auto total = 0LL;
    vector<int> b(m);
    vector<vector<int>> indices(n + 1);
    for (int i = 0; i < m; i++) {
        int r, a;
        cin >> r >> a >> b[i];

        total += a - b[i];
        indices[r].emplace_back(i);
    }

    vector<long long> sum(n + 1, 0);
    vector<int> subtree_size(n + 1), dist(n + 1, -1);
    vector<bool> removed(n + 1, false);
    queue<int> q;
    auto decompose = [&](auto &&self, int root = 1) -> void {
        auto centroid = [&]() {
            auto dfs1 = [&](auto &&self, int v, int prev = -1) -> void {
                subtree_size[v] = 1;
                for (int u : adj_list[v])
                    if (u != prev && !removed[u]) {
                        self(self, u, v);
                        subtree_size[v] += subtree_size[u];
                    }
            };
            dfs1(dfs1, root);

            auto dfs2 = [&](auto &&self, int size, int v, int prev = -1) -> int {
                for (int u : adj_list[v])
                    if (u != prev && !removed[u])
                        if (2 * subtree_size[u] > size) return self(self, size, u, v);
                return v;
            };
            return dfs2(dfs2, subtree_size[root], root);
        };
        int c = centroid();
        removed[c] = true;

        auto process = [&](int s, bool subtree = false) {
            vector<int> path{s};
            dist[s] = subtree;
            q.emplace(s);
            while (!q.empty()) {
                int v = q.front();
                q.pop();

                for (int u : adj_list[v])
                    if (!removed[u] && dist[u] == -1) {
                        q.emplace(u);
                        path.emplace_back(u);
                        dist[u] = dist[v] + 1;
                    }
            }

            vector<int> diff;
            for (int v : path)
                for (int i : indices[v])
                    if (b[i] >= dist[v]) diff.emplace_back(b[i] - dist[v]);
            sort(diff.begin(), diff.end());

            int k = diff.size();
            vector<long long> pref(k + 1, 0);
            for (int i = 0; i < k; i++) pref[i + 1] = pref[i] + diff[i];

            for (int v : path) {
                int i = lower_bound(diff.begin(), diff.end(), dist[v]) - diff.begin();
                if (i < k) sum[v] += (!subtree ? 1 : -1) * (pref[k] - pref[i] - (long long) dist[v] * (k - i));
                dist[v] = -1;
            }
        };
        process(c);

        for (int v : adj_list[c])
            if (!removed[v]) process(v, true);

        for (int v : adj_list[c])
            if (!removed[v]) self(self, v);
    };
    decompose(decompose);

    cout << total + *max_element(sum.begin(), sum.end());
}
