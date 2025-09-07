#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
    vector<int> sets;

    int find(int v) {
        return sets[v] == v ? v : (sets[v] = find(sets[v]));
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set != v_set) {
            sets[v_set] = u_set;
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

        int u = a, v = b + n, u_set = dsu.find(u), v_set = dsu.find(v);
        if (adj_matrix[u_set].size() < adj_matrix[v_set].size()) {
            swap(u, v);
            swap(u_set, v_set);
        }
        
        if (dsu.unite(u, v)) {
            int k = 0;
            if (adj_matrix[u_set].count(v_set)) k = adj_matrix[u_set][v_set];
            else if (adj_matrix[v_set].count(u_set)) k = adj_matrix[v_set][u_set];

            auto sum = [&](long long x) { return x * (x - 1) / 2; };

            count += sum(size[u_set] + size[v_set] - k) - sum(size[u_set]) - sum(size[v_set]);
            size[u_set] += size[v_set] - k;

            for (auto &[t, w] : adj_matrix[v_set]) {
                if (t == u_set) {
                    count += sum(w);
                    continue;
                }

                int s = 0;
                if (adj_matrix[u_set].count(t)) s = adj_matrix[u_set][t];
                count -= sum(s + w) - sum(s) - sum(w);
                adj_matrix[u_set][t] = s + w;

                if (adj_matrix[t].count(v_set)) adj_matrix[t].erase(v_set);
                adj_matrix[t][u_set] += w;
            }
            adj_matrix[u_set].erase(v_set);
            adj_matrix[v_set].clear();
        }
        cout << count << "\n";
    }
}
