#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, w;
    cin >> n >> m >> w;

    vector<vector<pair<int, int>>> adj_list(n + 1);
    while (m--) {
        int u, v, t;
        cin >> u >> v >> t;

        adj_list[u].emplace_back(v, t);
    }

    vector<int> profit(n + 1, INT_MIN);
    profit[1] = 0;
    queue<int> q;
    q.emplace(1);
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (auto &u : adj_list[v])
            if (profit[u.first] < min(w, u.second + profit[v])) {
                profit[u.first] = min(w, u.second + profit[v]);
                q.emplace(u.first);
            }
    }

    cout << profit[n];
}
