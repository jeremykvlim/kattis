#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj_list_regular(n), adj_list_transpose(n);
    while (m--) {
        int a, b;
        cin >> a >> b;

        adj_list_regular[a].emplace_back(b);
        adj_list_transpose[b].emplace_back(a);
    }

    vector<int> dist_regular(n, 1e9);
    dist_regular[0] = 0;
    queue<int> q;
    q.emplace(0);
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (int u : adj_list_regular[v])
            if (dist_regular[u] == 1e9) {
                dist_regular[u] = dist_regular[v] + 1;
                q.emplace(u);
            }
    }

    vector<int> dist_transpose(n, 1e9);
    dist_transpose[n - 1] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    pq.emplace(0, n - 1);
    while (!pq.empty()) {
        auto [d, v] = pq.top();
        pq.pop();

        if (d != dist_transpose[v]) continue;

        for (int u : adj_list_transpose[v]) {
            int du = max(dist_regular[u], d + 1);
            if (dist_transpose[u] > du) {
                dist_transpose[u] = du;
                pq.emplace(du, u);
            }
        }
    }
    dist_transpose.resize(n - 1);

    for (int d : dist_transpose) cout << d << " ";
}
