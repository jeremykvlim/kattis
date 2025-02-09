#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> inspections(n);
    for (int &i : inspections) cin >> i;

    vector<vector<int>> time(n, vector<int>(n));
    for (auto &row : time) {
        for (int &t : row) cin >> t;

        for (int i = 0; i < n; i++) row[i] += inspections[i];
    }

    vector<array<int, 4>> flights(m);
    for (auto &fl : flights) {
        int s, f, t;
        cin >> s >> f >> t;

        fl = {s, f, t, t + time[s - 1][f - 1]};
    }

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++) time[j][k] = min(time[j][k], time[j][i] + time[i][k]);

    vector<vector<int>> adj_list(m);
    for (int i = 0; i < m; i++)
        for (int j = 0; j < m; j++)
            if (flights[i][1] == flights[j][0] && flights[i][3] <= flights[j][2] ||
                flights[i][3] + time[flights[i][1] - 1][flights[j][0] - 1] <= flights[j][2]) adj_list[i].emplace_back(j);

    vector<int> match(m, -1);
    int count = 0;
    vector<int> visited(m, -1);
    for (int i = 0; i < m; i++) {
        auto dfs = [&](auto &&self, int v) {
            if (visited[v] == i) return false;

            visited[v] = i;
            for (int u : adj_list[v])
                if (match[u] == -1 || self(self, match[u])) {
                    match[u] = v;
                    return true;
                }

            return false;
        };
        if (dfs(dfs, i)) count++;
    }
    cout << m - count;
}
