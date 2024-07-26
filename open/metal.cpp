#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> dist(n, vector<int>(n));
    vector<int> d(n);
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++) {
            cin >> dist[i][j];

            dist[j][i] = dist[i][j];
            d[i] = max(d[i], dist[i][j]);
            d[j] = max(d[j], dist[i][j]);
        }

    vector<int> order(n);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int i, int j) {return d[i] > d[j];});

    int sum = 2e9;
    vector<int> state(n, 0);
    auto dfs = [&](auto &&self, int v = 0, int s1 = 0, int s2 = 0) {
        if (s1 + s2 >= sum) return;

        if (v == n) {
            sum = s1 + s2;
            return;
        }

        int d1 = 0, d2 = 0;
        for (int i = 0; i < v; i++)
            if (state[order[i]] == 1) d1 = max(d1, dist[order[v]][order[i]]);
            else d2 = max(d2, dist[order[v]][order[i]]);

        state[order[v]] = 1;
        self(self, v + 1, max(s1, d1), s2);
        state[order[v]] = 2;
        self(self, v + 1, s1, max(s2, d2));
    };

    dfs(dfs);
    cout << sum;
}
