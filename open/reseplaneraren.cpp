#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct FenwickTree {
    vector<T> BIT;

    void update(int i, T v) {
        for (; i && i < BIT.size(); i += i & -i) BIT[i] += v;
    }

    T pref_sum(int i) {
        T sum = 0;
        for (; i; i &= i - 1) sum += BIT[i];
        return sum;
    }

    T range_sum_query(int l, int r) {
        if (l >= r) return 0;
        return pref_sum(r) - pref_sum(l);
    }

    FenwickTree(int n) : BIT(n, 0) {}
};

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

    int n, k, q;
    cin >> n >> k >> q;

    vector<vector<int>> adj_list(n);
    for (int _ = 0; _ < n - 1; _++) {
        int u, v;
        cin >> u >> v;

        adj_list[u - 1].emplace_back(v - 1);
        adj_list[v - 1].emplace_back(u - 1);
    }

    vector<vector<int>> lift(__lg(n) + 1, vector<int>(n, 0));
    vector<int> depth(n, 0), in(n), out(n);
    int count = 0;
    auto dfs = [&](auto &&self, int v = 0, int prev = -1) -> void {
        in[v] = count++;
        if (~prev) {
            depth[v] = depth[prev] + 1;
            lift[0][v] = prev;
            for (int i = 1; i <= __lg(n); i++) lift[i][v] = lift[i - 1][lift[i - 1][v]];
        }

        for (int u : adj_list[v])
            if (u != prev) {
                depth[u] = depth[v] + 1;
                self(self, u, v);
            }
        out[v] = count;
    };
    dfs(dfs);

    auto ancestor = [&](int v, int u) {
        return in[v] <= in[u] && in[u] < out[v];
    };

    auto lca = [&](int u, int v) -> int {
        if (ancestor(u, v)) return u;
        if (ancestor(v, u)) return v;

        if (depth[u] < depth[v]) swap(u, v);

        for (int i = __lg(n); ~i; i--)
            if (!ancestor(lift[i][u], v)) u = lift[i][u];

        return lift[0][u];
    };

    vector<int> s(k), t(k), anc(k);
    for (int i = 0; i < k; i++) {
        cin >> s[i] >> t[i];
        s[i]--;
        t[i]--;

        anc[i] = lca(s[i], t[i]);
    }

    vector<int> order(k);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int i, int j) { return depth[anc[i]] < depth[anc[j]]; });

    DisjointSets dsu(n);
    vector<int> deepest(n, 1e9);
    for (int i : order) {
        for (int u = dsu.find(s[i]); u && depth[u] >= depth[anc[i]]; u = dsu.find(u)) {
            deepest[u] = depth[anc[i]];
            dsu.sets[u] = dsu.find(lift[0][u]);
        }

        for (int v = dsu.find(t[i]); v && depth[v] >= depth[anc[i]]; v = dsu.find(v)) {
            deepest[v] = depth[anc[i]];
            dsu.sets[v] = dsu.find(lift[0][v]);
        }
    }

    vector<int> queries(q, 0);
    vector<array<int, 8>> sweep;
    for (int i = 0; i < q; i++) {
        int a, b;
        cin >> a >> b;
        a--;
        b--;

        if (ancestor(a, b)) {
            if (deepest[b] <= depth[a]) queries[i]++;
        } else if (ancestor(b, a)) {
            if (deepest[a] <= depth[b]) queries[i]++;
        } else {
            int d = depth[lca(a, b)];
            sweep.push_back({d, in[a] - 1, 1, 0, in[b], out[b], i, -1});
            sweep.push_back({d, out[a] - 1, 1, 0, in[b], out[b], i, 1});
        }
    }

    if (!sweep.empty()) {
        for (int i = 0; i < k; i++) {
            sweep.push_back({depth[anc[i]], in[s[i]], 0, in[t[i]], 0, 0, 0, 0});
            sweep.push_back({depth[anc[i]], in[t[i]], 0, in[s[i]], 0, 0, 0, 0});
        }
        sort(sweep.begin(), sweep.end());

        FenwickTree<int> fw(n + 1);
        for (auto &[d, x, add, y, l, r, i, sgn] : sweep)
            if (add) queries[i] += sgn * fw.range_sum_query(l, r);
            else fw.update(y + 1, 1);
    }

    for (int possible : queries) cout << (possible ? "Yes\n" : "No\n");
}
