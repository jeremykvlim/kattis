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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, q;
    cin >> n >> m >> q;

    vector<vector<int>> adj_list(n);
    for (int _ = 0; _ < n - 1; _++) {
        int u, v;
        cin >> u >> v;

        adj_list[u - 1].emplace_back(v - 1);
        adj_list[v - 1].emplace_back(u - 1);
    }

    vector<int> degree(n, 0);
    vector<pair<int, int>> edges(m);
    for (auto &[u, v] : edges) {
        cin >> u >> v;
        u--;
        v--;

        degree[u]++;
        degree[v]++;
    }

    auto lsb = [&](int x) {
        return x & -x;
    };

    vector<pair<int, int>> tour;
    vector<int> index(n), depth(n, 0), in(n), out(n), anc_mask(n, 0), head(n + 1);
    vector<long long> degree_sum(n, 0);
    int count = 0;
    auto dfs1 = [&](auto &&self, int v = 0, int p = 0) -> void {
        tour.emplace_back(v, p);
        index[v] = in[v] = count++;

        for (int u : adj_list[v])
            if (u != p) {
                depth[u] = depth[v] + 1;
                degree_sum[u] = degree_sum[v] + degree[u];
                self(self, u, v);
                head[index[u]] = v;
                if (lsb(index[v]) < lsb(index[u])) index[v] = index[u];
            }
        out[v] = count;
    };
    dfs1(dfs1);
    for (auto [v, p] : tour) anc_mask[v] = anc_mask[p] | lsb(index[v]);

    auto lca = [&](int u, int v) -> int {
        if (unsigned above = in[u] ^ in[v]; above) {
            above = (anc_mask[u] & anc_mask[v]) & -bit_floor(above);
            if (unsigned below = anc_mask[u] ^ above; below) {
                below = bit_floor(below);
                u = head[(in[u] & -below) | below];
            }
            if (unsigned below = anc_mask[v] ^ above; below) {
                below = bit_floor(below);
                v = head[(in[v] & -below) | below];
            }
        }

        return depth[u] < depth[v] ? u : v;
    };

    vector<pair<int, int>> queries(q);
    vector<int> lcas(q);
    vector<long long> pipes(q);
    vector<vector<int>> indices(n);
    for (int i = 0; i < q; i++) {
        auto &[a, b] = queries[i];
        cin >> a >> b;
        a--;
        b--;

        lcas[i] = lca(a, b);
        pipes[i] = degree_sum[a] + degree_sum[b] - 2 * degree_sum[lcas[i]] + degree[lcas[i]];
        indices[a].emplace_back(i);
        indices[b].emplace_back(i);
    }

    vector<vector<int>> sweep(n);
    for (auto [u, v] : edges) {
        if (depth[u] >= depth[v]) swap(u, v);
        sweep[v].emplace_back(u);
    }

    FenwickTree<long long> fw(n + 1);
    vector<int> visits(n, 0);
    auto dfs2 = [&](auto &&self, int v = 0, int prev = -1) -> void {
        for (int u : sweep[v]) {
            fw.update(in[u] + 1, 1);
            fw.update(out[u] + 1, -1);
            visits[u]++;
        }

        for (int i : indices[v]) {
            auto [a, b] = queries[i];
            pipes[i] -= 2 * (fw.range_sum_query(in[lcas[i]] + 1, in[a] + 1) + fw.range_sum_query(in[lcas[i]] + 1, in[b] + 1) + visits[lcas[i]]);
        }

        for (int u : adj_list[v])
            if (u != prev) self(self, u, v);

        for (int u : sweep[v]) {
            fw.update(in[u] + 1, -1);
            fw.update(out[u] + 1, 1);
            visits[u]--;
        }
    };
    dfs2(dfs2);

    for (auto p : pipes) cout << p << "\n";
}
