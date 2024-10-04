#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    auto sum = 0LL;
    int biggest = -1;
    vector<vector<int>> table(n, vector<int>(n));
    for (auto &row : table)
        for (int &field : row) {
            cin >> field;

            biggest = max(biggest, field);
            sum += field;
        }

    vector<int> count(2 * biggest + 1, 0);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if (i + 1 < n) count[table[i][j] + table[i + 1][j]]++;
            if (j + 1 < n) count[table[i][j] + table[i][j + 1]]++;
        }

    for (int i = count.size() - 1, dominos = min(2 * n * (n - 1), 7 * (k - 1) + 1); ~i; i--) {
        count[i] = min(count[i], dominos);
        dominos -= count[i];
    }

    int id = 2;
    vector<vector<int>> coverable(n, vector<int>(n, -1));
    vector<array<int, 3>> process;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if (i + 1 < n && count[table[i][j] + table[i + 1][j]]-- > 0) {
                if (coverable[i][j] == -1) coverable[i][j] = id++;
                if (coverable[i + 1][j] == -1) coverable[i + 1][j] = id++;

                if ((i + j) & 1) process.push_back({coverable[i + 1][j], coverable[i][j], 0});
                else process.push_back({coverable[i][j], coverable[i + 1][j], 0});
            }

            if (j + 1 < n && count[table[i][j] + table[i][j + 1]]-- > 0) {
                if (coverable[i][j] == -1) coverable[i][j] = id++;
                if (coverable[i][j + 1] == -1) coverable[i][j + 1] = id++;

                if ((i + j) & 1) process.push_back({coverable[i][j + 1], coverable[i][j], 0});
                else process.push_back({coverable[i][j], coverable[i][j + 1], 0});
            }

            if (coverable[i][j] == -1) continue;

            if ((i + j) & 1) process.push_back({coverable[i][j], 1, -table[i][j]});
            else process.push_back({0, coverable[i][j], -table[i][j]});
        }

    vector<array<int, 3>> edges(2 * process.size());
    vector<int> capacity(2 * process.size(), 0), flow(2 * process.size(), 0);
    vector<vector<array<int, 3>>> adj_list(id);
    for (int i = 0; i < process.size(); i++) {
        auto [u, v, w] = process[i];

        edges[2 * i] = {u, v, w};
        edges[2 * i + 1] = {v, u, -w};
        capacity[2 * i] = 1;
        adj_list[u].push_back({v, w, 2 * i});
        adj_list[v].push_back({u, -w, 2 * i + 1});
    }

    vector<int> potential(id, INT_MAX);
    potential[0] = 0;

    auto bellman_ford = [&]() {
        for (int _ = 0; _ < id; _++)
            for (int i = 0; i < edges.size(); i++) {
                auto [u, v, w] = edges[i];
                if (capacity[i] != flow[i]) potential[v] = min(potential[v], potential[u] + w);
            }
    };
    bellman_ford();

    vector<bool> visited(id, false);
    auto dfs = [&](auto &&self, int v = 0) -> bool {
        visited[v] = true;

        if (v == 1) return true;

        for (auto [u, w, i] : adj_list[v]) {
            if (capacity[i] == flow[i] || potential[u] != potential[v] + w) continue;

            if (!visited[u] && self(self, u)) {
                flow[i]++;
                flow[i ^ 1]--;

                sum += w;
                return true;
            }
        }

        return false;
    };

    while (k--) {
        auto dijkstra = [&]() {
            vector<int> dist(id, INT_MAX);
            dist[0] = 0;
            priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
            pq.emplace(0, 0);
            while (!pq.empty()) {
                auto [d, v] = pq.top();
                pq.pop();

                for (auto [u, w, i] : adj_list[v]) {
                    if (capacity[i] == flow[i]) continue;

                    if (dist[u] > d + potential[v] + w - potential[u]) {
                        dist[u] = d + potential[v] + w - potential[u];
                        pq.emplace(dist[u], u);
                    }
                }
            }

            for (int i = 0; i < id; i++)
                if (dist[i] != INT_MAX) potential[i] += dist[i];
        };
        dijkstra();

        fill(visited.begin(), visited.end(), false);
        if (!dfs(dfs)) break;
    }

    cout << sum;
}
