#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct SparseTable {
    vector<vector<T>> ST;
    function<T(T, T)> f;

    SparseTable() {}
    SparseTable(const vector<T> &v, function<T(T, T)> func) : f(func) {
        if (v.empty()) return;
        ST.resize(__lg(v.size()) + 1);
        ST[0] = v;
        for (int i = 1; i < ST.size(); i++) {
            ST[i].resize(v.size() - (1 << i) + 1);
            for (int j = 0; j < ST[i].size(); j++) ST[i][j] = f(ST[i - 1][j], ST[i - 1][j + (1 << (i - 1))]);
        }
    }

    T range_query(int l, int r) {
        int i = __lg(r - l);
        return f(ST[i][l], ST[i][r - (1 << i)]);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<vector<int>> adj_list(n);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;

        adj_list[u - 1].emplace_back(v - 1);
        adj_list[v - 1].emplace_back(u - 1);
    }

    auto lsb = [&](int x) {
        return x & -x;
    };

    vector<pair<int, int>> tour;
    vector<int> child(n, -1), depth(n, 0), prev(n, 0), inlabel(n), ascendant(n, 0), head(n + 1);
    auto dfs = [&](auto &&self, int v = 0) -> void {
        tour.emplace_back(v, prev[v]);
        inlabel[v] = tour.size();

        for (int u : adj_list[v])
            if (u != prev[v]) {
                prev[u] = v;
                depth[u] = depth[v] + 1;
                self(self, u);
                head[inlabel[u]] = v;
                if (lsb(inlabel[v]) < lsb(inlabel[u])) inlabel[v] = inlabel[u];
            }
    };
    dfs(dfs);

    for (auto [v, p] : tour) {
        ascendant[v] = ascendant[p] | lsb(inlabel[v]);
        if (v && inlabel[v] == inlabel[p]) child[p] = v;
    }

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

    vector<array<pair<int, int>, 4>> dp1(n);
    vector<int> temp(n);
    for (int i = n - 1; ~i; i--) {
        int v = tour[i].first;
        for (int u : adj_list[v])
            if (prev[u] == v) {
                pair<int, int> p{temp[u] + 1, u};
                for (int j = 0; j < 4; j++)
                    if (dp1[v][j].first < p.first) {
                        for (int k = 3; k > j; k--) dp1[v][k] = dp1[v][k - 1];
                        dp1[v][j] = p;
                        break;
                    }
            }
        temp[v] = dp1[v][0].first;
    }

    vector<int> dp2(n, 0), sum(n);
    for (int i = 1; i < n; i++) {
        int v = tour[i].first, p = prev[v];
        dp2[v] = max(dp2[p], dp1[p][0].second == v ? dp1[p][1].first : dp1[p][0].first) + 1;
        if (dp1[p][0].second == v) sum[v] = dp1[p][1].first + dp1[p][2].first;
        else sum[v] = dp1[p][0].first + (dp1[p][1].second == v ? dp1[p][2].first : dp1[p][1].first);
    }

    vector<int> base_sum, base, offset(n + 1), pos(n);
    for (auto [v, p] : tour)
        if (!v || inlabel[v] != inlabel[p]) {
            offset[inlabel[v]] = base.size();
            for (int u = v; ~u; u = child[u]) {
                base_sum.emplace_back(sum[u]);
                base.emplace_back(u);
                pos[u] = base.size() - 1;
            }
        }

    SparseTable<int> st(base_sum, [](int x, int y) { return max(x, y); });
    auto climb = [&](int v, int d) -> pair<int, int> {
        int s = 0;
        while (d) {
            if (d <= pos[v] - offset[inlabel[v]]) {
                s = max(s, st.range_query(pos[v] - d + 1, pos[v] + 1));
                return {base[pos[v] - d], s};
            }
            d -= pos[v] - offset[inlabel[v]] + 1;
            s = max(s, st.range_query(offset[inlabel[v]], pos[v] + 1));
            v = head[inlabel[v]];
        }
        return {v, s};
    };

    while (q--) {
        int a, b;
        cin >> a >> b;
        a--;
        b--;

        int c = lca(a, b), da = depth[a] - depth[c], db = depth[b] - depth[c], ca = -1, cb = -1, extra = 0;
        if (da) {
            auto [v, s] = climb(a, da - 1);
            ca = v;
            extra = max({extra, s, dp1[a][0].first + dp1[a][1].first});
        }

        if (db) {
            auto [v, s] = climb(b, db - 1);
            cb = v;
            extra = max({extra, s, dp1[b][0].first + dp1[b][1].first});
        }

        int x = dp2[c], y = 0;
        for (auto [z, v] : dp1[c])
            if (v != ca && v != cb) {
                if (x < z) y = exchange(x, z);
                else y = max(y, z);
            }
        extra = max(extra, x + y);

        if (!extra) cout << "-1\n";
        else cout << da + db + 1 + extra << "\n";
    }
}
