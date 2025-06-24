#include <bits/stdc++.h>
using namespace std;

struct PersistentDisjointSets {
    vector<int> sets, size;
    deque<array<int, 4>> history;

    int find(int v) {
        return sets[v] == v ? v : find(sets[v]);
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set != v_set) {
            if (size[u_set] < size[v_set]) swap(u_set, v_set);
            sets[v_set] = u_set;
            size[u_set] += size[v_set];
            history.push_back({v_set, v_set, u_set, size[v_set]});
            return true;
        }
        return false;
    }

    int record() {
        return history.size();
    }

    void restore(int version = 0) {
        while (record() > version) {
            sets[history.back()[0]] = history.back()[1];
            size[history.back()[2]] -= history.back()[3];
            history.pop_back();
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

    vector<pair<int, int>> roads(m);
    for (auto &[u, v] : roads) cin >> u >> v;
    roads.insert(roads.begin(), {0, 0});

    int q;
    cin >> q;

    vector<pair<int, int>> junctions(q);
    for (auto &[s, t] : junctions) cin >> s >> t;

    PersistentDisjointSets pdsu(n + 1);
    vector<int> all(q), cost(q);
    iota(all.begin(), all.end(), 0);
    auto dfs = [&](auto &&self, int m1, int lg, vector<int> &days) {
        if (!~lg) {
            for (int d : days) cost[d] = m1;
            return;
        }

        int m2 = m1 ^ (1 << lg), version = pdsu.record();
        for (int mask = m2; mask; --mask &= m2) {
            auto [u, v] = roads[mask];
            pdsu.unite(u, v);
        }

        vector<int> same, diff;
        for (int d : days) {
            auto [s, t] = junctions[d];
            (pdsu.find(s) == pdsu.find(t) ? same : diff).emplace_back(d);
        }
        pdsu.restore(version);

        self(self, m2, lg - 1, same);
        self(self, m1, lg - 1, diff);
    };
    dfs(dfs, (1 << (__lg(m) + 1)) - 1, __lg(m), all);
    for (int c : cost) cout << c << "\n";
}
