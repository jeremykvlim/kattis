#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct SparseTable {
    vector<vector<T>> ST;
    function<T(T, T)> f;

    SparseTable() {}
    SparseTable(vector<T> v, function<T(T, T)> func) : f(move(func)) {
        if (v.empty()) return;
        int n = __lg(v.size()) + 1;
        ST.resize(n);
        ST.front() = v;
        for (int i = 1; i < n; i++) {
            ST[i].resize(v.size() - (1 << i) + 1);
            for (int j = 0; j <= v.size() - (1 << i); j++)
                ST[i][j] = f(ST[i - 1][j], ST[i - 1][j + (1 << (i - 1))]);
        }
    }

    T range_query(int l, int r) {
        int i = __lg(r - l + 1);
        return f(ST[i][l], ST[i][r - (1 << i) + 1]);
    }
};

pair<SparseTable<int>, vector<int>> lca_st(int n, vector<vector<int>> &adj_list) {
    vector<int> order(n + 1, 0), euler_tour, depth(2 * n);
    auto dfs = [&](auto &&self, int v = 0, int p = -1, int d = 0) -> void {
        euler_tour.emplace_back(v);
        depth[euler_tour.size()] = d;
        order[v] = euler_tour.size();
        for (int u : adj_list[v])
            if (u != p) {
                self(self, u, v, d + 1);
                euler_tour.emplace_back(v);
                depth[euler_tour.size()] = d;
            }
    };
    dfs(dfs);

    return {SparseTable<int>(depth, [](int x, int y) {return min(x, y);}), order};
}

int lca(int u, int v, SparseTable<int> &st, vector<int> &order) {
    auto [l, r] = minmax(order[u], order[v]);
    return st.range_query(l, r);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> id(n + 1, 0), parent(n + 1, 0), op(n + 1, -1);
    vector<vector<int>> adj_list(n + 1);
    vector<array<int, 3>> queries;
    for (int i = 1; i <= n; i++) {
        char c;
        int v;
        cin >> c >> v;

        if (c == 'a') {
            id[i] = i;
            parent[i] = id[v];
            adj_list[id[v]].emplace_back(i);
        } else if (c == 'b') {
            id[i] = parent[id[v]];
            op[i] = id[v];
        } else {
            int w;
            cin >> w;

            id[i] = id[v];
            queries.push_back({id[v], id[w], i});
        }
    }

    auto [st, order] = lca_st(n, adj_list);
    for (auto [v, w, i] : queries) op[i] = lca(v, w, st, order);

    for (int i = 1; i <= n; i++)
        if (op[i] != -1) cout << op[i] << "\n";
}
