#include <bits/stdc++.h>
using namespace std;

struct PersistentDisjointSet {
    vector<int> sets;
    stack<pair<int, int>> history;

    int find(int p) {
        return (sets[p] == p) ? p : (sets[p] = find(sets[p]));
    }

    bool unite(int p, int q) {
        int p_set = find(p), q_set = find(q);
        if (p_set != q_set) {
            sets[q_set] = p_set;
            history.emplace(q_set, q_set);
            return true;
        }
        return false;
    }

    int record() {
        return history.size();
    }

    void undo(int version) {
        while (record() > version) {
            sets[history.top().first] = history.top().second;
            history.pop();
        }
    }

    PersistentDisjointSet(int n) : sets(n) {
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

    PersistentDisjointSet pdsu(n + 1);
    vector<int> all(q), cost(q);
    iota(all.begin(), all.end(), 0);
    auto dfs = [&](auto &&self, int curr, int lg, vector<int> &days) {
        if (lg < 0) {
            for (int d : days) cost[d] = curr;
            return;
        }

        int next = curr ^ (1 << lg), version = pdsu.record();
        for (int i = next; i; --i &= next) {
            auto [u, v] = roads[i];
            pdsu.unite(u, v);
        }

        vector<int> same, diff;
        for (int d : days) {
            auto [s, t] = junctions[d];
            (pdsu.find(s) == pdsu.find(t) ? same : diff).emplace_back(d);
        }
        pdsu.undo(version);
        
        self(self, next, lg - 1, same);
        self(self, curr, lg - 1, diff);
    };
    dfs(dfs, (1 << (__lg(m) + 1)) - 1, __lg(m), all);

    for (int c : cost) cout << c << "\n";
}
