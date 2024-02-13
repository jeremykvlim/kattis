#include <bits/stdc++.h>
using namespace std;

void dfs(int v, vector<vector<int>> &adj_list, vector<bool> &visited, vector<int> &l) {
    l.emplace_back(v);
    visited[v] = true;
    for (int u : adj_list[v])
        if (!visited[u]) dfs(u, adj_list, visited, l);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, e;
    cin >> n >> e;

    vector<vector<int>> adj_list(n);
    while (e--) {
        int a, b;
        cin >> a >> b;

        adj_list[a].emplace_back(b);
        adj_list[b].emplace_back(a);
    }

    vector<int> L, order(n, -1);
    int v;
    while (cin >> v) {
        if (order[v] != -1 || !(0 <= v && v < n)) {
            cout << "NO";
            exit(0);
        }

        order[v] = L.size();
        L.emplace_back(v);
    }

    if (L.size() ^ n) {
        cout << "NO";
        exit(0);
    }

    for (auto &al : adj_list) sort(al.begin(), al.end(), [&](int u, int v) {return order[u] < order[v];});

    vector<int> l;
    vector<bool> visited(n, false);
    dfs(L[0], adj_list, visited, l);

    cout << (l == L ? "YES" : "NO");
}
