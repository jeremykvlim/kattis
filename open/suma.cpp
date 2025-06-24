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

    int n;
    cin >> n;

    vector<vector<int>> h(n, vector<int>(n)), V(n, vector<int>(n));
    for (auto &row : h)
        for (int &hij : row) cin >> hij;

    for (auto &row : V)
        for (int &vij : row) cin >> vij;

    PersistentDisjointSets dsu(n * n);
    vector<tuple<double, int, int>> edges;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if (i < n - 1) {
                int h1 = h[i][j], h2 = h[i + 1][j], v1 = V[i][j], v2 = V[i + 1][j];
                if (v1 != v2) {
                    auto t = (double) (h2 - h1) / (v1 - v2);
                    if (t >= 0) edges.emplace_back(t, i * n + j, (i + 1) * n + j);
                } else if (h1 == h2) dsu.unite(i * n + j, (i + 1) * n + j);
            }

            if (j < n - 1) {
                int h1 = h[i][j], h2 = h[i][j + 1], v1 = V[i][j], v2 = V[i][j + 1];
                if (v1 != v2) {
                    auto t = (double) (h2 - h1) / (v1 - v2);
                    if (t >= 0) edges.emplace_back(t, i * n + j, i * n + j + 1);
                } else if (h1 == h2) dsu.unite(i * n + j, i * n + j + 1);
            }
        }
    sort(edges.begin(), edges.end());

    double prev = -1;
    int size = 0, version = dsu.record();
    for (auto [t, u, v] : edges) {
        if (t != prev) dsu.restore(version);
        dsu.unite(u, v);
        size = max(size, dsu.size[dsu.find(u)]);
        prev = t;
    }
    cout << size;
}
