#include <bits/stdc++.h>
using namespace std;

struct DisjointSet {
    vector<int> sets;

    int find(int p) {
        return (sets[p] == p) ? p : (sets[p] = find(sets[p]));
    }

    bool unite(int p, int q) {
        int p_set = find(p), q_set = find(q);
        if (p_set != q_set) {
            sets[q_set] = p_set;
            return true;
        }
        return false;
    }

    DisjointSet(int n) : sets(n) {
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

    DisjointSet dsu(n + 1);
    vector<int> all(q), cost(q);
    iota(all.begin(), all.end(), 0);
    auto dnc = [&](auto &&self, int curr, int lg, vector<int> &days) {
        if (lg < 0) {
            for (int d : days) cost[d] = curr;
            return;
        }

        int next = curr ^ (1 << lg);
        vector<int> undo;
        for (int i = next; i; --i &= next) {
            auto [u, v] = roads[i];
            dsu.unite(u, v);

            undo.emplace_back(u);
            undo.emplace_back(v);
        }

        vector<int> same, diff;
        for (int d : days) {
            auto [s, t] = junctions[d];
            (dsu.find(s) == dsu.find(t) ? same : diff).emplace_back(d);
        }

        for (int i : undo) dsu.sets[i] = i;
        self(self, next, lg - 1, same);
        self(self, curr, lg - 1, diff);
    };
    dnc(dnc, (1 << (__lg(m) + 1)) - 1, __lg(m), all);

    for (int c : cost) cout << c << "\n";
}
