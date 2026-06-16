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
        while (sets[v] >= 0) {
            int p = sets[v];
            if (sets[p] >= 0) sets[v] = sets[p];
            v = p;
        }
        return v;
    }

    pair<int, int> unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set == v_set) return {u_set, -1};

        if (sets[u_set] > sets[v_set]) swap(u_set, v_set);
        sets[u_set] += sets[v_set];
        sets[v_set] = u_set;
        return {u_set, v_set};
    }

    int size(int v) {
        return -sets[find(v)];
    }

    DisjointSets(int n) : sets(n, -1) {}
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

    auto lsb = [&](int x) {
        return x & -x;
    };

    vector<pair<int, int>> tour;
    vector<int> depth(n, 0), in(n), out(n), prev(n, 0), inlabel(n), ascendant(n, 0), head(n + 1);
    int count = 0;
    auto dfs = [&](auto &&self, int v = 0) -> void {
        tour.emplace_back(v, prev[v]);
        inlabel[v] = tour.size();
        in[v] = count++;

        for (int u : adj_list[v])
            if (u != prev[v]) {
                prev[u] = v;
                depth[u] = depth[v] + 1;
                self(self, u);
                head[inlabel[u]] = v;
                if (lsb(inlabel[v]) < lsb(inlabel[u])) inlabel[v] = inlabel[u];
            }
        out[v] = count;
    };
    dfs(dfs);
    for (auto [v, p] : tour) ascendant[v] = ascendant[p] | lsb(inlabel[v]);

    auto lca = [&](int u, int v) -> int {
        if (unsigned above = inlabel[u] ^ inlabel[v]; above) {
            above = (ascendant[u] & ascendant[v]) & -bit_floor(above);
            if (unsigned below = ascendant[u] ^ above; below) {
                below = bit_floor(below);
                u = head[(inlabel[u] & -below) | below];
            }
            if (unsigned below = ascendant[v] ^ above; below) {
                below = bit_floor(below);
                v = head[(inlabel[v] & -below) | below];
            }
        }

        return depth[u] < depth[v] ? u : v;
    };

    auto ancestor = [&](int u, int v) {
        return lca(u, v) == u;
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
    vector<int> deepest(n, 1e9), rep(n);
    iota(rep.begin(), rep.end(), 0);
    for (int i : order) {
        for (int u = rep[dsu.find(s[i])]; u && depth[u] >= depth[anc[i]]; u = rep[dsu.find(u)]) {
            deepest[u] = depth[anc[i]];
            int temp = rep[dsu.find(prev[u])];
            auto [big, small] = dsu.unite(prev[u], u);
            if (small != -1) rep[big] = temp;
        }

        for (int v = rep[dsu.find(t[i])]; v && depth[v] >= depth[anc[i]]; v = rep[dsu.find(v)]) {
            deepest[v] = depth[anc[i]];
            int temp = rep[dsu.find(prev[v])];
            auto [big, small] = dsu.unite(prev[v], v);
            if (small != -1) rep[big] = temp;
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
