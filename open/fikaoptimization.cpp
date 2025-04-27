#include <bits/stdc++.h>
using namespace std;

struct CentroidDecomposition {
    vector<vector<int>> adj_list;
    vector<bool> removed;
    vector<int> subtree_size, depth;
    vector<long long> sum;

    CentroidDecomposition(int n) : adj_list(n), removed(n, false), subtree_size(n, 0), depth(n, -1), sum(n, 0) {}

    void add_edge(int u, int v) {
        adj_list[u].emplace_back(v);
        adj_list[v].emplace_back(u);
    }

    void decompose(const vector<int> &b, const vector<vector<int>> &indices, int root = 1) {
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
        int c = dfs2(dfs2, subtree_size[root], root);
        removed[c] = true;

        queue<int> q;
        auto process = [&](int s, bool subtree = false) {
            vector<int> path{s};
            depth[s] = subtree;
            q.emplace(s);
            while (!q.empty()) {
                int v = q.front();
                q.pop();

                for (int u : adj_list[v])
                    if (!removed[u] && depth[u] == -1) {
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

        for (int v : adj_list[c])
            if (!removed[v]) decompose(b, indices, v);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    CentroidDecomposition cd(n + 1);
    for (int _ = 0; _ < n - 1; _++) {
        int u, v;
        cin >> u >> v;

        cd.add_edge(u, v);
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

    cd.decompose(b, indices);
    cout << total + *max_element(cd.sum.begin(), cd.sum.end());
}
