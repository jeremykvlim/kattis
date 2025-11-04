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

    int total = 0;
    __int128 RG = 0, BG = 0;
    vector<array<int, 3>> edges(m);
    for (int i = 0; i < m; i++) {
        char c;
        cin >> edges[i][0] >> edges[i][1] >> edges[i][2] >> c;

        total += edges[i][2];
        if (c != 'B') RG |= (__int128) 1 << i;
        if (c != 'R') BG |= (__int128) 1 << i;
    }

    auto connected = [&](auto &subgraph) {
        DisjointSets dsu(n + 1);
        int count = 0;
        for (int i = 0; i < m; i++)
            if ((subgraph >> i) & 1) {
                auto [a, b, w] = edges[i];
                if (dsu.unite(a, b)) count++;
            }
        return count == n - 1;
    };

    if (n > 1)
        if (!connected(RG) || !connected(BG)) {
            while (m--) cout << "-1\n";
            exit(0);
        }

    int deficit = 0;
    vector<int> sums(m + 1, 1e9);
    sums[m] = total;
    for (unsigned __int128 deleted = 0; popcount(deleted) <= m;) {
        vector<int> weight(m + 2, 0);
        vector<vector<int>> adj_list(m + 2);
        for (int v = 0; v < m; v++) {
            auto build = [&](auto &subgraph) -> pair<int, vector<int>> {
                DisjointSets dsu(n + 1);
                int count = 0;
                for (int i = 0; i < m; i++)
                    if ((subgraph >> i) & 1 && i != v && !((deleted >> i) & 1)) {
                        auto [a, b, w] = edges[i];
                        if (dsu.unite(a, b)) count++;
                    }

                vector<int> component(n + 1);
                for (int i = 1; i <= n; i++) component[i] = dsu.find(i);
                return {count, component};
            };

            if (!((deleted >> v) & 1)) {
                auto [count_r, component_r] = build(RG);
                if (n == 1 || count_r == n - 1) adj_list[m].emplace_back(v);
                if (count_r == n - 2) {
                    for (int i = 0; i < m; i++)
                        if ((deleted >> i) & 1 && (RG >> i) & 1) {
                            auto [a, b, w] = edges[i];
                            if (component_r[a] != component_r[b]) adj_list[i].emplace_back(v);
                        }
                }

                auto [count_b, component_b] = build(BG);
                if (n == 1 || count_b == n - 1) adj_list[v].emplace_back(m + 1);
                if (count_b == n - 2) {
                    for (int i = 0; i < m; i++)
                        if ((deleted >> i) & 1 && (BG >> i) & 1) {
                            auto [a, b, w] = edges[i];
                            if (component_b[a] != component_b[b]) adj_list[v].emplace_back(i);
                        }
                }
                weight[v] = -edges[v][2];
            } else weight[v] = edges[v][2];
        }

        vector<int> dist(m + 2, 1e9), prev(m + 2, -1);
        dist[m] = 0;
        vector<bool> queued(m + 2, false);
        queued[m] = true;
        deque<int> dq{m};
        while (!dq.empty()) {
            int v = dq.front();
            dq.pop_front();

            queued[v] = false;

            for (int u : adj_list[v])
                if (dist[u] > dist[v] + weight[u]) {
                    dist[u] = dist[v] + weight[u];
                    prev[u] = v;

                    if (!queued[u]) {
                        if (dq.empty() || dist[u] < dist[dq.front()]) dq.emplace_front(u);
                        else dq.emplace_back(u);
                        queued[u] = true;
                    }
                }
        }
        if (dist[m + 1] == 1e9) break;

        vector<int> path;
        for (int v = m + 1; ~v; v = prev[v]) path.emplace_back(v);
        path.pop_back();
        reverse(path.begin(), path.end());
        path.pop_back();

        for (int v : path)
            if (0 <= v && v < m) {
                if (!((deleted >> v) & 1)) deficit += edges[v][2];
                else deficit -= edges[v][2];

                deleted ^= (__int128) 1 << v;
            }
        sums[m - popcount(deleted)] = total - deficit;
    }

    for (int k = 1; k <= m; k++)
        if (sums[k] == 1e9) cout << "-1\n";
        else cout << sums[k] << "\n";
}
