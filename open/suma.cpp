#include <bits/stdc++.h>
using namespace std;

struct RollbackDisjointSets {
    vector<int> sets;
    vector<pair<int, int>> history;

    int find(int v) {
        while (sets[v] >= 0) v = sets[v];
        return v;
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set == v_set) return false;

        if (sets[u_set] > sets[v_set]) swap(u_set, v_set);
        history.emplace_back(v_set, sets[v_set]);
        sets[u_set] += sets[v_set];
        sets[v_set] = u_set;
        return true;
    }

    int size(int v) {
        return -sets[find(v)];
    }

    int record() {
        return history.size();
    }

    void rollback(int version) {
        while (record() > version) {
            auto [v_set, s] = history.back();
            history.pop_back();

            int u_set = sets[v_set];
            sets[u_set] -= s;
            sets[v_set] = s;
        }
    }

    void delete_history(int version = 0) {
        history.resize(version);
    }

    RollbackDisjointSets(int n) : sets(n, -1) {}
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

    RollbackDisjointSets rdsu(n * n);
    vector<tuple<double, int, int>> edges;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if (i < n - 1) {
                int h1 = h[i][j], h2 = h[i + 1][j], v1 = V[i][j], v2 = V[i + 1][j];
                if (v1 != v2) {
                    auto t = (double) (h2 - h1) / (v1 - v2);
                    if (t >= 0) edges.emplace_back(t, i * n + j, (i + 1) * n + j);
                } else if (h1 == h2) rdsu.unite(i * n + j, (i + 1) * n + j);
            }

            if (j < n - 1) {
                int h1 = h[i][j], h2 = h[i][j + 1], v1 = V[i][j], v2 = V[i][j + 1];
                if (v1 != v2) {
                    auto t = (double) (h2 - h1) / (v1 - v2);
                    if (t >= 0) edges.emplace_back(t, i * n + j, i * n + j + 1);
                } else if (h1 == h2) rdsu.unite(i * n + j, i * n + j + 1);
            }
        }
    sort(edges.begin(), edges.end());

    double prev = -1;
    int size = 0, version = rdsu.record();
    for (auto [t, u, v] : edges) {
        if (t != prev) rdsu.rollback(version);
        rdsu.unite(u, v);
        size = max(size, rdsu.size(u));
        prev = t;
    }
    cout << size;
}
