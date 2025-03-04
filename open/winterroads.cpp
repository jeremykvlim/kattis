#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
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

    DisjointSets(int n) : sets(n) {
        iota(sets.begin(), sets.end(), 0);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<array<int, 3>> edges(m);
    for (auto &[a, b, c] : edges) cin >> a >> b >> c;

    auto cmp = [&](int i, int j) {return edges[i][2] != edges[j][2] ? edges[i][2] > edges[j][2] : i < j;};
    set<int, decltype(cmp)> roads(cmp);
    for (int i = 0; i < m; i++) roads.emplace(i);

    int e;
    cin >> e;

    deque<array<int, 4>> q;
    auto process = [&]() {
        sort(q.rbegin(), q.rend());
        vector<bool> path(q.size(), false);
        DisjointSets dsu(n + 1);
        auto it = roads.begin();
        while (!q.empty()) {
            auto [w, a, b, i] = q.front();
            q.pop_front();

            while (it != roads.end() && edges[*it][2] >= w) {
                dsu.unite(edges[*it][0], edges[*it][1]);
                it = next(it);
            }

            path[i] = dsu.find(a) == dsu.find(b);
        }

        for (bool b : path) cout << b << "\n";
    };
    while (e--) {
        char ch;
        cin >> ch;

        if (ch == 'S') {
            int a, b, w;
            cin >> a >> b >> w;

            q.push_back({w, a, b, (int) q.size()});
        } else {
            process();

            int r, c;
            cin >> r >> c;

            roads.erase(r - 1);
            edges[r - 1][2] = c;
            roads.emplace(r - 1);
        }
    }
    process();
}
