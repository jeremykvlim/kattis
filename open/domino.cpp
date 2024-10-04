#include <bits/stdc++.h>
using namespace std;

void dijkstra(vector<vector<array<int, 6> *>> adj_list, vector<int> &potential) {
    vector<int> dist(potential.size(), INT_MAX);
    dist[0] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    pq.emplace(0, 0);
    while (!pq.empty()) {
        auto [d, v] = pq.top();
        pq.pop();

        for (auto e : adj_list[v]) {
            auto [u, v, w, capacity, flow, index] = *e;

            if (capacity == flow) continue;

            if (dist[v] > d + w + potential[u] - potential[v]) {
                dist[v] = d + w + potential[u] - potential[v];
                pq.emplace(dist[v], v);
            }
        }
    }

    for (int i = 0; i < dist.size(); i++)
        if (dist[i] != INT_MAX) potential[i] += dist[i];
}

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

    vector<array<int, 6>> edges;
    int index = 0;
    auto add = [&](int u, int v, int w) {
        edges.push_back({u, v, w, 1, 0, index++});
        edges.push_back({v, u, -w, 0, 0, index++});
    };

    int id = 2;
    vector<vector<int>> coverable(n, vector<int>(n, -1));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if (i + 1 < n && count[table[i][j] + table[i + 1][j]]-- > 0) {
                if (coverable[i][j] == -1) coverable[i][j] = id++;
                if (coverable[i + 1][j] == -1) coverable[i + 1][j] = id++;

                if ((i + j) & 1) add(coverable[i + 1][j], coverable[i][j], 0);
                else add(coverable[i][j], coverable[i + 1][j], 0);
            }

            if (j + 1 < n && count[table[i][j] + table[i][j + 1]]-- > 0) {
                if (coverable[i][j] == -1) coverable[i][j] = id++;
                if (coverable[i][j + 1] == -1) coverable[i][j + 1] = id++;

                if ((i + j) & 1) add(coverable[i][j + 1], coverable[i][j], 0);
                else add(coverable[i][j], coverable[i][j + 1], 0);
            }

            if (coverable[i][j] == -1) continue;

            if ((i + j) & 1) add(coverable[i][j], 1, -table[i][j]);
            else add(0, coverable[i][j], -table[i][j]);
        }

    vector<vector<array<int, 6> *>> adj_list(id);
    for (auto &e : edges) adj_list[e[1]].emplace_back(&e);

    vector<int> potential(id, INT_MAX);
    potential[0] = 0;

    auto bellman_ford = [&]() {
        for (int i = 0; i < id; i++)
            for (auto [u, v, w, capacity, flow, index] : edges)
                if (capacity != flow) potential[v] = min(potential[v], potential[u] + w);
    };
    bellman_ford();

    vector<bool> visited(id, false);
    auto dfs = [&](auto &&self, int v = 0) -> bool {
        visited[v] = true;

        if (v == 1) return true;

        for (auto e : adj_list[v]) {
            auto &[u, v, w, capacity, flow, index] = *e;
            if (capacity == flow || potential[v] != potential[u] + w) continue;

            if (!visited[v] && self(self, v)) {
                flow++;
                edges[index ^ 1][4]--;

                sum += w;
                return true;
            }
        }

        return false;
    };

    while (k--) {
        dijkstra(adj_list, potential);

        fill(visited.begin(), visited.end(), false);
        if (!dfs(dfs)) break;
    }

    cout << sum;
}
