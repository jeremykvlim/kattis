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

vector<array<int, 3>> kruskal(int n, vector<array<int, 3>> edges) {
    DisjointSet dsu(n);
    sort(edges.begin(), edges.end());

    vector<array<int, 3>> mst;
    for (auto e : edges) {
        auto [w, u, v] = e;
        if (dsu.unite(u, v)) mst.emplace_back(e);
    }

    return mst;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, w;
    cin >> n >> w;

    vector<vector<int>> c(n, vector<int>(n)), b(n, vector<int>(n));
    auto read = [&](vector<vector<int>> &a) {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < i; j++) {
                cin >> a[i][j];

                a[j][i] = a[i][j];
            }
    };
    read(c);
    read(b);

    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            for (int k = 0; k < n; k++) {
                if (i != k && j != k) {
                    auto check = [&](vector<vector<int>> a) {
                        return min(a[i][k], a[k][j]) > a[i][j];
                    };

                    if (check(c) || check(b)) {
                        cout << "NO";
                        exit(0);
                    }
                }
            }

    vector<array<int, 3>> edges_c, edges_b;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (c[i][j] + b[i][j] >= w) {
                edges_c.push_back({-c[i][j], i, j});
                edges_b.push_back({-b[i][j], i, j});
            }

    auto mst_c = kruskal(n, edges_c), mst_b = kruskal(n, edges_b);
    if (mst_c.size() != n - 1 || mst_b.size() != n - 1) {
        cout << "NO";
        exit(0);
    }

    cout << mst_c.size() + mst_b.size() << "\n";
    for (auto [d, u, v] : mst_c) cout << u << " " << v << " " << w + d << "\n";
    for (auto [d, u, v] : mst_b) cout << u << " " << v << " " << -d << "\n";
}
