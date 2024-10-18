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

    int n, m, u;
    cin >> n >> m >> u;

    vector<array<int, 3>> roads(m);
    for (auto &[x, y, t] : roads) cin >> x >> y >> t;
    sort(roads.begin(), roads.end(), [](auto r1, auto r2) {return r1[2] > r2[2];});

    vector<array<int, 4>> queries(u);
    for (int i = 0; i < u; i++) {
        int a, b, p;
        cin >> a >> b >> p;

        queries[i] = {a, b, p, i};
    }
    sort(queries.begin(), queries.end(), [](auto q1, auto q2) {return q1[2] < q2[2];});

    DisjointSet dsu(n + 1);
    vector<bool> safe(u);
    for (auto [a, b, p, i] : queries) {
        while (!roads.empty() && roads.back()[2] <= p) {
            auto [x, y, t] = roads.back();
            roads.pop_back();

            dsu.unite(x, y);
        }

        safe[i] = dsu.find(a) == dsu.find(b);
    }

    for (bool s : safe) cout << (s ? "TAIP\n" : "NE\n");
}
