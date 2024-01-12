#include <bits/stdc++.h>
using namespace std;

bool dfs(int v, vector<int> &match, vector<bool> &visited, vector<vector<int>> &adj_list) {
    if (visited[v]) return false;

    visited[v] = true;
    for (int u : adj_list[v])
        if (match[u] == -1 || dfs(match[u], match, visited, adj_list)) {
            match[u] = v;
            return true;
        }

    return false;
}

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

    vector<tuple<int, int, int, int>> flights(m);
    for (auto &fl : flights) {
        int s, f, t;
        cin >> s >> f >> t;

        fl = {s, f, t, t + time[s - 1][f - 1]};
    }

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                time[j][k] = min(time[j][k], time[j][i] + time[i][k]);

    vector<vector<int>> adj_list(m);
    for (int i = 0; i < m; i++)
        for (int j = 0; j < m; j++)
            if (get<1>(flights[i]) == get<0>(flights[j]) && get<3>(flights[i]) <= get<2>(flights[j]) ||
                get<3>(flights[i]) + time[get<1>(flights[i]) - 1][get<0>(flights[j]) - 1] <= get<2>(flights[j])) adj_list[i].emplace_back(j);

    vector<int> match(m, -1);
    int count = 0;
    for (int i = 0; i < m; i++) {
        vector<bool> visited(m, false);
        if (dfs(i, match, visited, adj_list)) count++;
    }

    cout << m - count;
}
