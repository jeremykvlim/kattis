#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
    vector<int> sets;

    int find(int v) {
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

    DisjointSets(int n) : sets(n, -1) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    int total = 0;
    vector<int> c(n);
    for (int &ci : c) {
        cin >> ci;

        total += ci;
    }

    DisjointSets dsu(n);
    while (m--) {
        int a, b;
        cin >> a >> b;

        dsu.unite(a - 1, b - 1);
    }

    vector<int> sum(n, 0);
    for (int i = 0; i < n; i++) sum[dsu.find(i)] += c[i];

    vector<bool> contaminated(n, false);
    for (int i = 0; i < k; i++) {
        int p;
        cin >> p;

        contaminated[dsu.find(p - 1)] = true;
    }

    for (int i = 0; i < n; i++)
        if (dsu.find(i) == i && contaminated[i]) total -= sum[i];
    cout << total;
}
