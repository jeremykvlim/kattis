#include <bits/stdc++.h>
using namespace std;

struct PersistentDisjointSets {
    vector<int> sets, size;
    vector<pair<int, int>> history;

    int find(int v) {
        return sets[v] == v ? v : find(sets[v]);
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set != v_set) {
            if (size[u_set] < size[v_set]) swap(u_set, v_set);
            history.push_back({v_set, size[v_set]});
            sets[v_set] = u_set;
            size[u_set] += size[v_set];
            return true;
        }
        return false;
    }

    int record() {
        return history.size();
    }

    void restore(int version) {
        while (record() > version) {
            auto [v_set, s] = history.back();
            history.pop_back();
            int u_set = sets[v_set];
            sets[v_set] = v_set;
            size[u_set] -= s;
        }
    }

    void delete_history(int version = 0) {
        history.resize(version);
    }

    PersistentDisjointSets(int n) : sets(n), size(n, 1) {
        iota(sets.begin(), sets.end(), 0);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    int lg = __lg(m), mask = (1 << (lg + 1)) - 1;
    vector<array<int, 2>> roads(mask + 1, {0, 0});
    for (int i = 1; i <= m; i++) cin >> roads[i][0] >> roads[i][1];

    int q;
    cin >> q;

    vector<array<int, 2>> junctions(q);
    for (auto &[s, t] : junctions) cin >> s >> t;

    vector<int> order(q), cost(q);
    iota(order.begin(), order.end(), 0);

    PersistentDisjointSets pdsu(n + 1);
    auto dfs = [&](auto &&self, int l, int r, int m1, int b) -> void {
        if (l >= r) return;
        if (!~b) {
            for (int i = l; i < r; i++) cost[order[i]] = m1;
            return;
        }

        int m2 = m1 ^ (1 << b), version = pdsu.record();
        for (int m3 = m2; m3; --m3 &= m2) {
            auto [u, v] = roads[m3];
            pdsu.unite(u, v);
        }

        int i = l, j = r;
        while (i < j) {
            auto [s, t] = junctions[order[i]];
            if (pdsu.find(s) == pdsu.find(t)) i++;
            else {
                j--;
                swap(order[i], order[j]);
            }
        }
        pdsu.restore(version);

        self(self, l, i, m2, b - 1);
        self(self, i, r, m1, b - 1);
    };
    dfs(dfs, 0, q, mask, lg);
    for (int c : cost) cout << c << "\n";
}
