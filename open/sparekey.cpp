#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> adj_list_transpose(n + 1);
    for (int i = 1; i <= n; i++) {
        int m;
        cin >> m;

        while (m--) {
            int s;
            cin >> s;

            adj_list_transpose[s].emplace_back(i);
        }
    }

    vector<int> e(n + 1);
    for (int i = n; i; i--) cin >> e[i];

    vector<bool> visited(n + 1, false);
    vector<int> g(n + 1, n);
    queue<int> q;
    for (int i = 1, count = 0; i <= n; i++) {
        if (!visited[e[i]]) {
            visited[e[i]] = true;
            count++;
            q.emplace(e[i]);
            while (!q.empty()) {
                int v = q.front();
                q.pop();

                for (int u : adj_list_transpose[v])
                    if (!visited[u]) {
                        visited[u] = true;
                        count++;
                        q.emplace(u);
                    }
            }
        }
        g[i] = n - count;
    }
    for (int i = n; i; i--) cout << g[i - 1] << " ";
}
