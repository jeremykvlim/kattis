#include <bits/stdc++.h>
using namespace std;

template <typename F>
void centroid_decomposition(const vector<vector<int>> &adj_list, F &&f) {
    vector<int> subtree_size(adj_list.size());
    vector<bool> removed(adj_list.size(), false);
    auto decompose = [&](auto &&self, int root = 1) -> void {
        auto dfs = [&](auto &&self, int v, int prev = -1) -> void {
            subtree_size[v] = 1;
            for (int u : adj_list[v])
                if (u != prev && !removed[u]) {
                    self(self, u, v);
                    subtree_size[v] += subtree_size[u];
                }
        };
        dfs(dfs, root);
        int c = root;
        for (int size = subtree_size[root], prev = -1, next = -1;; prev = c, c = next, next = -1) {
            for (int u : adj_list[c])
                if (u != prev && !removed[u] && 2 * subtree_size[u] > size) {
                    next = u;
                    break;
                }
            if (!~next) break;
        }
        removed[c] = true;

        f(c, removed);
        for (int v : adj_list[c])
            if (!removed[v]) self(self, v);
    };
    decompose(decompose);
}

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

    vector<int> depth(n + 1, -1);
    vector<long long> sum(n + 1, 0);
    centroid_decomposition(adj_list, [&](int c, const auto &removed) {
        queue<int> q;
        auto process = [&](int s, bool subtree = false) {
            vector<int> path{s};
            depth[s] = subtree;
            q.emplace(s);
            while (!q.empty()) {
                int v = q.front();
                q.pop();

                for (int u : adj_list[v])
                    if (!removed[u] && !~depth[u]) {
                        q.emplace(u);
                        path.emplace_back(u);
                        depth[u] = depth[v] + 1;
                    }
            }

            vector<int> diff;
            for (int v : path)
                for (int i : indices[v])
                    if (b[i] >= depth[v]) diff.emplace_back(b[i] - depth[v]);
            sort(diff.begin(), diff.end());

            int k = diff.size();
            vector<long long> pref(k + 1, 0);
            for (int i = 0; i < k; i++) pref[i + 1] = pref[i] + diff[i];

            for (int v : path) {
                int i = lower_bound(diff.begin(), diff.end(), depth[v]) - diff.begin();
                if (i < k) sum[v] += (!subtree ? 1 : -1) * (pref[k] - pref[i] - (long long) depth[v] * (k - i));
                depth[v] = -1;
            }
        };
        process(c);

        for (int v : adj_list[c])
            if (!removed[v]) process(v, true);
    });
    cout << total + *max_element(sum.begin(), sum.end());
}
