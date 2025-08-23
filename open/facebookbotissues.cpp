#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    vector<int> degree(n, 0);
    vector<vector<int>> adj_list(n);
    while (m--) {
        int x, y;
        cin >> x >> y;
        x--;
        y--;

        adj_list[x].emplace_back(y);
        adj_list[y].emplace_back(x);
        degree[x]++;
        degree[y]++;
    }

    if (k == 1) {
        if (any_of(degree.begin(), degree.end(), [&](int d) { return d >= 2; })) {
            cout << "YES";
            exit(0);
        }

        cout << "NO";
        exit(0);
    }

    vector<int> candidates;
    for (int i = 0; i < n; i++) {
        sort(adj_list[i].begin(), adj_list[i].end());
        if (degree[i] >= k) candidates.emplace_back(i);
    }

    if (candidates.size() < 2) {
        cout << "NO";
        exit(0);
    }

    vector<int> seen(n + 1, 0);
    for (int i = 0, count = 1; i < candidates.size(); i++)
        for (int j = i + 1; j < candidates.size(); j++) {
            int u = candidates[i], v = candidates[j];
            if (min(degree[u], degree[v]) < k) continue;

            if (degree[u] > degree[v]) swap(u, v);
            for (int t : adj_list[u]) seen[t] = count;
            int c = 0;
            for (int t : adj_list[v])
                if (seen[t] == count) {
                    if (++c >= k) {
                        cout << "YES";
                        exit(0);
                    }
                }
            count++;
        }

    cout << "NO";
}
