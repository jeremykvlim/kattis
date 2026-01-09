#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj_list(n + 1);
    while (m--) {
        int u, v;
        cin >> u >> v;

        adj_list[u].emplace_back(v);
        adj_list[v].emplace_back(u);
    }

    int x;
    cin >> x;

    mt19937 rng(random_device{}());
    vector<int> visited(n + 1, -1);
    int count = 0;
    for (int i = 0; i < 5e4; i++) {
        int v;
        do v = rng() % n + 1;
        while (v == x);
        visited[v] = i;
        while (v != x && !adj_list[v].empty()) {
            int u = adj_list[v][rng() % adj_list[v].size()];
            if (visited[u] == i) break;
            visited[v = u] = i;
        }
        count += v == x;
    }
    cout << fixed << setprecision(1) << (n - 1) * count / 5e4 + 1;
}
