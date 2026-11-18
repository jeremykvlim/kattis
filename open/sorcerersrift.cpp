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

        int c = root, size = subtree_size[root];
        for (int prev = -1, next = -1;; prev = c, c = next, next = -1) {
            for (int u : adj_list[c])
                if (u != prev && !removed[u] && 2 * subtree_size[u] > size) {
                    next = u;
                    break;
                }
            if (!~next) break;
        }
        removed[c] = true;

        f(c, removed, [&](int v) { return subtree_size[v] < subtree_size[c] ? subtree_size[v] : size - subtree_size[c]; });
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

    vector<vector<int>> adj_list_h(n + 1), adj_list_l(m + 1);
    for (int _ = 0; _ < n - 1; _++) {
        int a, b;
        cin >> a >> b;

        adj_list_h[a].emplace_back(b);
        adj_list_h[b].emplace_back(a);
    }
    for (int _ = 0; _ < m - 1; _++) {
        int c, d;
        cin >> c >> d;

        adj_list_l[c - n].emplace_back(d - n);
        adj_list_l[d - n].emplace_back(c - n);
    }

    auto instruct = [&](int w, int x, int y, int z) {
        cout << "? " << w << " " << x << " " << y << " " << z << "\n" << flush;

        int k;
        cin >> k;
        return k;
    };

    auto location = [&](const vector<vector<int>> &adj_list, int x, int z) {
        centroid_decomposition(adj_list, [&](int c, auto &removed, auto &&component_size) {
            vector<int> candidates;
            for (int v : adj_list[c])
                if (!removed[v]) candidates.emplace_back(v);
            sort(candidates.begin(), candidates.end(), [&](int u, int v) { return component_size(u) > component_size(v); });

            int u = -1;
            if (!candidates.empty()) {
                int v = candidates.size() > 1 ? candidates[1] : c, w = z + candidates[0], k = instruct(w, x, w, z + v);
                if (k == 1) u = candidates[0];
                else if (k == 3) u = v;
            }

            if (~u) {
                for (int v : adj_list[c])
                    if (v != u) removed[v] = true;
            } else if (candidates.size() > 2) {
                removed[c] = false;
                removed[candidates[0]] = removed[candidates[1]] = true;
            } else {
                z += c;
                fill(removed.begin(), removed.end(), true);
            }
        });
        return z;
    };
    int u = location(adj_list_h, n + 1, 0), v = location(adj_list_l, 1, n);
    cout << "! " << u << " " << v;
}
