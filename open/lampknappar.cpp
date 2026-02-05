#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> adj_list_regular(n + 1), adj_list_transpose(n + 1);
    for (int i = 1; i <= n; i++) {
        int s;
        cin >> s;

        while (s--) {
            int a;
            cin >> a;

            adj_list_regular[i].emplace_back(a);
            adj_list_transpose[a].emplace_back(i);
        }
    }

    vector<vector<int>> dist1(n + 1, vector<int>(n + 1, -1));
    vector<bool> visited(n + 1, false);
    auto bfs = [&](int s) {
        queue<int> q;
        q.emplace(s);
        dist1[s][s] = 0;
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (int u : adj_list_regular[v])
                if (!~dist1[s][u]) {
                    dist1[s][u] = dist1[s][v] + 1;
                    q.emplace(u);
                }
        }
        visited[s] = true;
    };
    bfs(1);
    bfs(n);

    if (!~dist1[1][n] || !~dist1[n][1]) {
        cout << "nej";
        exit(0);
    }

    int bound = dist1[1][n] + dist1[n][1];
    vector<vector<int>> dist2(n + 1, vector<int>(n + 1, 1e9));
    dist2[n][n] = 0;
    vector<queue<pair<int, int>>> buckets(bound + 1);
    buckets[0].emplace(n, n);
    for (int d = 0;;) {
        for (; d <= bound && buckets[d].empty(); d++);
        if (d > bound) break;

        auto relax = [&](int u, int v, int w) {
            if (dist2[u][v] > d + w && bound >= d + w) {
                dist2[u][v] = d + w;
                buckets[d + w].emplace(u, v);
                if (u == 1 && v == 1) bound = d + w;
            }
        };

        auto [u, v] = buckets[d].front();
        buckets[d].pop();

        if (d != dist2[u][v]) continue;

        if (u == 1 && v == 1) {
            cout << d;
            exit(0);
        }

        for (int t : adj_list_transpose[u]) relax(t, v, t != v);
        for (int t : adj_list_regular[v]) relax(u, t, u != t);
        if (u != v) {
            if (!visited[v]) bfs(v);
            if (~dist1[v][u]) relax(v, u, dist1[v][u] - 1);
        }
    }
}
