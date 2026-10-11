#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<array<int, 2>> edges;
    vector<vector<int>> adj_list(12), edge_id(12, vector<int>(12, -1));
    auto add_edge = [&](int u, int v) {
        adj_list[u].emplace_back(v);
        adj_list[v].emplace_back(u);
        edges.push_back({u, v});
        edge_id[u][v] = edge_id[v][u] = edges.size() - 1;
    };

    for (int i = 0; i < 5; i++) add_edge(0, i + 1);
    for (int i = 0; i < 5; i++) add_edge(i + 1, (i + 1) % 5 + 1);
    for (int i = 0; i < 5; i++) {
        add_edge((i + 1) % 5 + 1, i + 6);
        add_edge((i + 1) % 5 + 1, (i + 1) % 5 + 6);
    }
    for (int i = 0; i < 5; i++) add_edge(i + 6, (i + 1) % 5 + 6);
    for (int i = 0; i < 5; i++) add_edge(11, i + 6);

    queue<int> q;
    vector<vector<int>> dist(12, vector<int>(12, -1));
    for (int s = 0; s < 12; s++) {
        dist[s][s] = 0;
        q.emplace(s);
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (int u : adj_list[v])
                if (!~dist[s][u]) {
                    dist[s][u] = dist[s][v] + 1;
                    q.emplace(u);
                }
        }
    }

    array<array<int, 3>, 2> targets{{{0, 2, 1}, {1, 2, 0}}};
    for (int v : adj_list[0])
        if (v != 1 && ~edge_id[2][v]) targets[0][2] = v;

    array<array<int, 30>, 2> permutations;
    for (int k = 0; k < 2; k++) {
        array<int, 12> pos{};
        for (int u = 0; u < 12; u++)
            for (int v = 0; v < 12; v++) {
                for (int t = 0; t < 3; t++)
                    if (dist[u][t] != dist[v][targets[k][t]]) goto next;
                pos[u] = v;
                break;
                next:;
            }

        for (int i = 0; i < 30; i++) {
            auto [u, v] = edges[i];
            permutations[k][i] = edge_id[pos[u]][pos[v]];
        }
    }

    array<int, 3> sections{};
    for (auto &mask : sections) {
        int m;
        cin >> m;

        while (m--) {
            int p;
            cin >> p;

            mask |= 1 << (p - 1);
        }
    }

    auto rotations = [&](int mask) {
        unordered_set<int> seen;
        auto dfs = [&](auto &&self, int mask) -> void {
            if (!seen.emplace(mask).second) return;
            for (auto &p : permutations) {
                int state = 0;
                for (int j = 0; j < 30; j++)
                    if ((mask >> j) & 1) state |= 1 << p[j];
                self(self, state);
            }
        };
        dfs(dfs, mask);
        return seen;
    };
    auto third = rotations(sections[2]);
    for (int state : rotations(sections[1]))
        if (!(sections[0] & state) && third.count(((1 << 30) - 1) ^ (sections[0] | state))) {
            cout << "Yes";
            exit(0);
        }
    cout << "No";
}