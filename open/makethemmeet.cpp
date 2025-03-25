#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
    vector<int> sets;

    int find(int v) {
        return sets[v] == v ? v : (sets[v] = find(sets[v]));
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set != v_set) {
            sets[v_set] = u_set;
            return true;
        }
        return false;
    }

    DisjointSets(int n) : sets(n) {
        iota(sets.begin(), sets.end(), 0);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj_list1(n);
    while (m--) {
        int u, v;
        cin >> u >> v;

        adj_list1[u].emplace_back(v);
        adj_list1[v].emplace_back(u);
    }

    int a = -1, b = -1;
    for (int v = 0; v < n; v++)
        for (int u = 0; u < n; u++) {
            vector<bool> only_adj_to_v(n, false);
            for (int t : adj_list1[v]) only_adj_to_v[t] = true;
            if (!only_adj_to_v[u]) continue;
            for (int t : adj_list1[u]) only_adj_to_v[t] = false;

            for (int t = n; ~t; t--)
                if (only_adj_to_v[t] && t != u) {
                    a = v;
                    b = u;
                    goto done;
                }
        }

    done:;
    if (!~a && !~b) {
        int temp = n;
        if (!(n & 1)) n++;

        vector<int> order(n), c(n);
        for (int i = 0; i < n; i++) {
            order[i] = i;
            if (2 * i <= n) c[i] = c[n - i - 1] = i;
        }

        cout << 2 * n << "\n";
        for (int _ = 0; _ < n; _++) {
            for (int __ = 0; __ < 2; __++) {
                for (int j = 0; j < temp; j++) cout << c[order[j]] << " ";
                cout << "\n";
            }

            swap(order[0], order[n - 1]);
            for (int j = 1; j < n - 1; j++) swap(order[j], order[j - 1]);
        }
        exit(0);
    }

    DisjointSets dsu(n);
    dsu.unite(a, b);

    vector<vector<int>> adj_list2(n);
    adj_list2[a].emplace_back(b);
    adj_list2[b].emplace_back(a);
    vector<bool> visited(n, false);
    for (int v : adj_list1[b]) visited[v] = true;
    for (int v : adj_list1[a])
        if (v != b && !visited[v] && dsu.unite(a, v)) {
            adj_list2[a].emplace_back(v);
            adj_list2[v].emplace_back(a);
            for (int u : adj_list1[v]) visited[u] = true;
        }

    for (int v = 0; v < n; v++)
        if (v != a)
            for (int u : adj_list1[v])
                if (u != a && dsu.unite(v, u)) {
                    adj_list2[v].emplace_back(u);
                    adj_list2[u].emplace_back(v);
                }

    vector<vector<int>> moves(600, vector<int>(n));
    for (auto &v : moves) iota(v.begin(), v.end(), 0);
    for (int i = 0; i < 200; i++) moves[2 * i][adj_list2[a][i & 1]] = a;

    for (int i = 0; i < adj_list2[a].size(); i++) {
        int j = i ? 1 : 3, step = i > 1 ? 2 : 4;

        vector<int> subtree_size(n, 1);
        auto dfs1 = [&](auto &&self, int v, int prev, int depth = 0) -> void {
            int k = j - depth;
            j += step;

            for (int u : adj_list2[v])
                if (u != prev) {
                    self(self, u, v, depth + 1);
                    subtree_size[v] += subtree_size[u];
                }

            for (int _ = 0; _ < subtree_size[v]; _++) moves[k += step][v] = prev;
        };
        dfs1(dfs1, adj_list2[a][i], a);
    }

    for (int v : adj_list2[a])
        for (int i = 1; i < 400; i += 2) moves[i][v] = a;

    int i = 400, next = a;
    auto dfs2 = [&](auto &&self, int v, int prev = -1) -> void {
        auto update = [&]() {
            moves[i++][v] = next;
            next = v;
        };

        if (v != next) update();

        for (int u : adj_list2[v])
            if (u != prev) {
                self(self, u, v);
                update();
            }
    };
    dfs2(dfs2, a);

    cout << "600\n";
    for (auto v : moves) {
        for (int c : v) cout << c << " ";
        cout << "\n";
    }
}
