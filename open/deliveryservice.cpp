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

    pair<int, int> unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set == v_set) return {u_set, -1};

        if (sets[u_set] > sets[v_set]) swap(u_set, v_set);
        sets[u_set] += sets[v_set];
        sets[v_set] = u_set;
        return {u_set, v_set};
    }

    int size(int v) {
        return -sets[find(v)];
    }

    DisjointSets(int n) : sets(n, -1) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<unordered_map<int, int>> adj_matrix(2 * n);
    for (int a = 0; a < n; a++) adj_matrix[a][a + n] = adj_matrix[a + n][a] = 1;

    DisjointSets dsu(2 * n);
    vector<int> size(2 * n, 1);
    auto count = 0LL;
    while (m--) {
        int a, b;
        cin >> a >> b;
        a--;
        b--;

        auto [big, small] = dsu.unite(a, b + n);
        if (small != -1) {
            int k = 0;
            if (adj_matrix[big].count(small)) k = adj_matrix[big][small];
            else if (adj_matrix[small].count(big)) k = adj_matrix[small][big];

            auto sum = [&](long long x) { return x * (x - 1) / 2; };

            count += sum(size[big] + size[small] - k) - sum(size[big]) - sum(size[small]);
            size[big] += size[small] - k;

            for (auto [t, w] : adj_matrix[small]) {
                if (t == big) {
                    count += sum(w);
                    continue;
                }

                int s = 0;
                if (adj_matrix[big].count(t)) s = adj_matrix[big][t];
                count -= sum(s + w) - sum(s) - sum(w);
                adj_matrix[big][t] = s + w;

                if (adj_matrix[t].count(small)) adj_matrix[t].erase(small);
                adj_matrix[t][big] += w;
            }
            adj_matrix[big].erase(small);
            adj_matrix[small].clear();
        }
        cout << count << "\n";
    }
}