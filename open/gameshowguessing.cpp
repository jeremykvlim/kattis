#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<int, int>> points(n);
    vector<int> xs(n), ys(n);
    for (int i = 0; i < n; i++) {
        auto &[x, y] = points[i];
        cin >> x >> y;

        xs[i] = x;
        ys[i] = y;
    }
    sort(xs.begin(), xs.end());
    sort(ys.begin(), ys.end());
    xs.erase(unique(xs.begin(), xs.end()), xs.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());

    int m = xs.size() + ys.size();
    vector<vector<pair<int, int>>> adj_list(m);
    vector<int> degree(m, 0);
    for (int i = 0; i < n; i++) {
        auto [x, y] = points[i];
        int u = lower_bound(xs.begin(), xs.end(), x) - xs.begin(), v = xs.size() + (lower_bound(ys.begin(), ys.end(), y) - ys.begin());
        adj_list[u].emplace_back(v, i);
        adj_list[v].emplace_back(u, i);
        degree[u]++;
        degree[v]++;
    }

    vector<int> dist(m, 0);
    queue<int> q;
    for (int v = 0; v < m; v++)
        if (degree[v] == 1) {
            dist[v] = 1;
            q.emplace(v);
        }

    vector<int> visited(n, -1);
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (auto [u, i] : adj_list[v])
            if (!~visited[i]) {
                visited[i] = v;
                degree[v]--;
                if (--degree[u] == 1) {
                    dist[u] = dist[v] + 1;
                    q.emplace(u);
                }
            }
    }

    int j = -1;
    for (int i = 0, d = -1; i < n; i++) {
        if (!~visited[i]) {
            cout << i + 1;
            exit(0);
        }

        if (d < dist[visited[i]]) {
            d = dist[visited[i]];
            j = i;
        }
    }
    cout << j + 1;
}
