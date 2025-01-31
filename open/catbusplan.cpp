#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    if (k > m) {
        cout << "Impossible";
        exit(0);
    }

    vector<pair<int, int>> roads(m);
    vector<vector<pair<int, int>>> adj_list(n + 1);
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;

        roads[i] = {a, b};
        adj_list[a].emplace_back(b, i);
        adj_list[b].emplace_back(a, i);
    }

    queue<int> q;
    vector<int> component(n + 1, -1);
    int count = 0;
    for (int i = 1; i <= n; i++)
        if (!~component[i]) {
            q.emplace(i);
            component[i] = count;
            while (!q.empty()) {
                int v = q.front();
                q.pop();

                for (auto [u, _] : adj_list[v])
                    if (!~component[u]) {
                        component[u] = count;
                        q.emplace(u);
                    }
            }
            count++;
        }

    vector<int> edge_counts(count, 0);
    vector<vector<int>> vertices(count), edges(count);
    for (int i = 1; i <= n; i++) vertices[component[i]].emplace_back(i);
    for (int i = 0; i < m; i++) {
        auto [a, b] = roads[i];
        if (component[a] == component[b]) {
            edges[component[a]].emplace_back(i);
            edge_counts[component[a]]++;
        }
    }

    vector<long long> degree(n + 1, 0LL);
    for (auto [a, b] : roads) {
        degree[a]++;
        degree[b]++;
    }

    int possible = 0;
    for (int c = 0; c < count; c++) {
        if (edges[c].empty()) continue;

        int odd = 0;
        for (int v : vertices[c]) odd += degree[v] & 1;

        if (!odd) possible++;
        else possible += odd / 2;
    }

    if (k < possible) {
        cout << "Impossible";
        exit(0);
    }

    vector<vector<int>> routes;
    for (int c = 0; c < count; c++) {
        if (vertices.empty() || edges[c].empty()) continue;

        vector<int> indices(n + 1);
        int V = vertices[c].size(), E = edges[c].size();
        for (int i = 0; i < V; i++) indices[vertices[c][i]] = i;

        vector<pair<int, int>> temp_edges(E);
        vector<vector<pair<int, int>>> temp_adj_list(V);
        for (int i = 0; i < edges[c].size(); i++) {
            int j = edges[c][i];
            auto [a, b] = roads[j];
            temp_edges[i] = {a, b};

            int u = indices[a], v = indices[b];
            temp_adj_list[u].emplace_back(v, i);
            temp_adj_list[v].emplace_back(u, i);
        }

        vector<int> temp_degree(V, 0);
        for (int i = 0; i < edges[c].size(); i++) {
            auto [a, b] = temp_edges[i];
            temp_degree[indices[a]]++;
            temp_degree[indices[b]]++;
        }

        vector<int> odd;
        for (int v = 0; v < V; v++)
            if (temp_degree[v] & 1) odd.emplace_back(v);

        int o = odd.size();
        vector<bool> used(edges[c].size(), false);
        if (o) {
            int half = o / 2;
            for (int i = 0; i < half; i++) {
                int l = odd[2 * i], r = odd[2 * i + 1];

                used.emplace_back(false);
                temp_edges.emplace_back(vertices[c][l], vertices[c][r]);
                temp_adj_list[l].emplace_back(r, temp_edges.size() - 1);
                temp_adj_list[r].emplace_back(l, temp_edges.size() - 1);
            }
        }

        int first = -1;
        for (int i = 0; i < V; i++)
            if (!temp_adj_list[i].empty()) {
                first = i;
                break;
            }
        if (!~first) continue;

        stack<int> sv, se;
        sv.emplace(first);
        se.emplace(-1);
        vector<int> vertex_indices, edge_indices;
        while (!sv.empty()) {
            int v = sv.top();
            if (!temp_adj_list[v].empty()) {
                while (!temp_adj_list[v].empty() && used[temp_adj_list[v].back().second]) temp_adj_list[v].pop_back();

                if (!temp_adj_list[v].empty()) {
                    auto [u, i] = temp_adj_list[v].back();
                    temp_adj_list[v].pop_back();

                    sv.emplace(u);
                    se.emplace(i);
                    used[i] = true;
                    continue;
                }
            }
            sv.pop();
            int i = se.top();
            se.pop();

            vertex_indices.emplace_back(v);
            if (~i) edge_indices.emplace_back(i);
        }

        int l = 0;
        for (int r = 0; r < edge_indices.size(); r++)
            if (edge_indices[r] >= edge_counts[c]) {
                if (l < r) {
                    vector<int> route;
                    for (int i = l; i <= r; i++) route.emplace_back(vertices[c][vertex_indices[i]]);
                    routes.emplace_back(route);
                }
                l = r + 1;
            }

        if (l + 1 <= edge_indices.size()) {
            vector<int> route;
            for (int i = l; i <= edge_indices.size(); i++) route.emplace_back(vertices[c][vertex_indices[i]]);
            routes.emplace_back(route);
        }
    }

    while (routes.size() < k) {
        int split = -1;
        for (int i = 0; i < routes.size(); i++)
            if (routes[i].size() >= 3) {
                split = i;
                break;
            }
        if (!~split) break;

        vector<int> temp = routes[split], l, r;
        int half = (temp.size() - 1) / 2;
        for (int i = 0; i <= half; i++) l.emplace_back(temp[i]);
        for (int i = half; i < temp.size(); i++) r.emplace_back(temp[i]);
        routes[split] = l;
        routes.emplace_back(r);
    }

    if (routes.size() != k) {
        cout << "Impossible";
        exit(0);
    }

    cout << "Possible\n";
    for (int i = 0; i < k; i++) {
        for (int c : routes[i]) cout << c << " ";
        cout << "\n";
    }
}
