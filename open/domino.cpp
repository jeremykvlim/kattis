#include <bits/stdc++.h>
using namespace std;

void dijkstra(int count, vector<vector<tuple<int, int, int, int, int, int> *>> adj_list, vector<int> &potential) {
    vector<int> dist(count, INT_MAX);
    dist[0] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    pq.emplace(0, 0);
    while (!pq.empty()) {
        auto [d, v] = pq.top();
        pq.pop();

        for (auto e : adj_list[v]) {
            auto [v, u, w, capacity, flow, index] = *e;

            if (capacity == flow) continue;

            if (dist[u] > d + w + potential[v] - potential[u]) {
                dist[u] = d + w + potential[v] - potential[u];
                pq.emplace(dist[u], u);
            }
        }
    }

    for (int i = 0; i < count; i++)
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

    vector<int> overlap(2 * biggest + 1, 0);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if (i + 1 < n) overlap[table[i][j] + table[i + 1][j]]++;
            if (j + 1 < n) overlap[table[i][j] + table[i][j + 1]]++;
        }

    for (int i = overlap.size() - 1, j = min(2 * n * (n - 1), 7 * (k - 1) + 1); ~i; i--) {
        overlap[i] = min(overlap[i], j);
        j -= overlap[i];
    }

    vector<tuple<int, int, int, int, int, int>> edges;
    int index = 0;
    auto add = [&](int u, int v, int w) {
        edges.emplace_back(u, v, w, 1, 0, index++);
        edges.emplace_back(v, u, -w, 0, 0, index++);
    };

    int count = 2;
    vector<vector<int>> id(n, vector<int>(n, -1));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if (i + 1 < n && overlap[table[i][j] + table[i + 1][j]]-- > 0) {
                if (id[i][j] == -1) id[i][j] = count++;
                if (id[i + 1][j] == -1) id[i + 1][j] = count++;

                if (i + j & 1) add(id[i + 1][j], id[i][j], 0);
                else add(id[i][j], id[i + 1][j], 0);
            }

            if (j + 1 < n && overlap[table[i][j] + table[i][j + 1]]-- > 0) {
                if (id[i][j] == -1) id[i][j] = count++;
                if (id[i][j + 1] == -1) id[i][j + 1] = count++;

                if (i + j & 1) add(id[i][j + 1], id[i][j], 0);
                else add(id[i][j], id[i][j + 1], 0);
            }

            if (id[i][j] == -1) continue;

            if (i + j & 1) add(id[i][j], 1, -table[i][j]);
            else add(0, id[i][j], -table[i][j]);
        }

    vector<vector<tuple<int, int, int, int, int, int> *>> adj_list(count);
    for (auto &e : edges) adj_list[get<0>(e)].emplace_back(&e);

    vector<int> potential(count, INT_MAX);
    potential[0] = 0;

    auto bellman_ford = [&]() {
        for (int i = 0; i < count; i++)
            for (auto [u, v, w, capacity, flow, index] : edges)
                if (capacity != flow) potential[v] = min(potential[v], potential[u] + w);
    };

    bellman_ford();

    vector<bool> visited(count, false);
    auto dfs = [&](auto &&self, int v = 0) -> bool {
        visited[v] = true;

        if (v == 1) return true;

        for (auto e : adj_list[v]) {
            auto &[v, u, w, capacity, flow, index] = *e;
            if (capacity == flow || w + potential[v] != potential[u]) continue;

            if (!visited[u] && self(self, u)) {
                flow++;
                get<4>(edges[index ^ 1])--;

                sum += w;
                return true;
            }
        }

        return false;
    };

    while (k--) {
        dijkstra(count, adj_list, potential);

        fill(visited.begin(), visited.end(), false);
        dfs(dfs);
    }

    cout << sum;
}
