#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> w(n + 1), V(n + 1);
    for (int i = 1; i <= n; i++) cin >> w[i];
    for (int i = 1; i <= n; i++) cin >> V[i];

    vector<vector<int>> adj_list(n + 1);
    while (m--) {
        int i, j;
        cin >> i >> j;

        adj_list[i].emplace_back(j);
        adj_list[j].emplace_back(i);
    }

    auto value = 0LL;
    queue<int> q;
    vector<bool> visited(n + 1, false);
    for (int s = 1; s <= n; s++) {
        if (!visited[s]) {
            q.emplace(s);
            visited[s] = true;
            vector<long long> a, b;
            while (!q.empty()) {
                int v = q.front();
                q.pop();

                a.emplace_back(w[v]);
                b.emplace_back(V[v]);

                for (int u : adj_list[v])
                    if (!visited[u]) {
                        visited[u] = true;
                        q.emplace(u);
                    }
            }
            sort(a.begin(), a.end());
            sort(b.begin(), b.end());
            value += inner_product(a.begin(), a.end(), b.begin(), 0LL);
        }
    }
    cout << value;
}