#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
    int t;
    vector<int> sets, seen;

    int find(int v) {
        if (seen[v] != t) {
            seen[v] = t;
            sets[v] = -1;
            return v;
        }

        while (sets[v] >= 0) {
            int p = sets[v];
            if (sets[p] >= 0) sets[v] = sets[p];
            v = p;
        }
        return v;
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set == v_set) return false;

        if (sets[u_set] > sets[v_set]) swap(u_set, v_set);
        sets[u_set] += sets[v_set];
        sets[v_set] = u_set;
        return true;
    }

    int size(int v) {
        return -sets[find(v)];
    }

    void reset() {
        t++;
    }

    DisjointSets(int n) : t(1), sets(n, -1), seen(n, 0) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<array<int, 2>> roads(m + 1);
    for (int i = 1; i <= m; i++) cin >> roads[i][0] >> roads[i][1];

    int Q;
    cin >> Q;

    vector<int> from, to(Q);
    vector<array<int, 2>> junctions(Q);
    for (int q = 0; q < Q; q++) {
        auto &[s, t] = junctions[q];
        cin >> s >> t;

        if (s != t) from.emplace_back(q);
    }

    DisjointSets dsu(n + 1);
    vector<int> component(n + 1), cost(Q);
    auto dfs = [&](auto &&self, int b, int l, int r, vector<int> &from, vector<int> &to, int m1 = 0) -> void {
        if (l >= r || !~b) return;

        dsu.reset();
        for (int m2 = m1, len = (1 << b) - 1;; --m2 &= m1) {
            for (int i = max(m2, 1); i <= min(m2 + len, m); i++) dsu.unite(roads[i][0], roads[i][1]);
            if (!m2) break;
        }

        int mid = l, right = r;
        if (n < 2 * (r - l)) {
            for (int i = 1; i <= n; i++) component[i] = dsu.find(i);
            for (int i = l; i < r; i++) {
                int q = from[i];
                if (component[junctions[q][0]] == component[junctions[q][1]]) to[mid++] = q;
                else {
                    cost[q] |= 1 << b;
                    to[--right] = q;
                }
            }
        } else
            for (int i = l; i < r; i++) {
                int q = from[i];
                if (dsu.find(junctions[q][0]) == dsu.find(junctions[q][1])) to[mid++] = q;
                else {
                    cost[q] |= 1 << b;
                    to[--right] = q;
                }
            }

        self(self, b - 1, l, mid, to, from, m1);
        self(self, b - 1, mid, r, to, from, m1 | (1 << b));
    };
    dfs(dfs, __lg(m), 0, from.size(), from, to);
    for (int c : cost) cout << c << "\n";
}
