#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj_list(n + 1);
    while (m--) {
        int v1, v2;
        cin >> v1 >> v2;

        adj_list[v1].emplace_back(v2);
        adj_list[v2].emplace_back(v1);
    }

    vector<int> a(n), indices(n + 1);
    for (int i = 0; i < n; i++) {
        cin >> a[i];

        indices[a[i]] = i;
    }
    for (int v = 1; v <= n; v++) sort(adj_list[v].begin(), adj_list[v].end(), [&](int a1, int a2) { return indices[a1] < indices[a2]; });

    vector<bool> bread(n + 1, false);
    bool change;
    do {
        change = false;

        vector<bool> visited(n + 1, false);
        deque<int> dq{1};
        for (int i = 0; !dq.empty() && i < n;) {
            int v = dq.front();
            dq.pop_front();

            if (visited[v]) continue;
            visited[v] = true;

            if (v != a[i]) {
                bread[a[i]] = change = true;
                break;
            }

            for (int j = adj_list[v].size() - 1; ~j; j--) {
                int u = adj_list[v][j];
                if (!visited[u] && bread[u]) dq.emplace_front(u);
            }

            for (int u : adj_list[v])
                if (!visited[u] && !bread[u]) dq.emplace_back(u);
            i++;
        }
    } while (change);
    cout << count_if(bread.begin(), bread.end(), [&](bool b) { return b; });
}
