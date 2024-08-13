#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj_list_regular(n + 1), adj_list_transpose(n + 1);
    while (m--) {
        int a, b;
        cin >> a >> b;

        adj_list_regular[a].emplace_back(b);
        adj_list_transpose[b].emplace_back(a);
    }

    vector<bool> visited1(n + 1, false), visited2(n + 1, false);
    visited1[1] = visited2[2] = true;
    queue<int> q;
    q.emplace(1);
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (int u : adj_list_regular[v])
            if (!visited1[u]) {
                visited1[u] = true;
                q.emplace(u);
            }
    }

    q.emplace(2);
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (int u : adj_list_transpose[v])
            if (!visited2[u]) {
                visited2[u] = true;
                q.emplace(u);
            }
    }

    int count = 0;
    for (int i = 1; i <= n; i++) {
        if (!(visited1[i] && visited2[i])) visited1[i] = false;
        if (visited1[i]) count++;
    }

    vector<int> indegree(n + 1, 0), dp(n + 1, 0);
    dp[1] = 1;
    for (int v = 1; v <= n; v++)
        if (visited1[v])
            for (int u : adj_list_regular[v])
                if (visited1[u]) indegree[u]++;

    q.emplace(1);
    bool zero = false;
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        count--;
        for (int u : adj_list_regular[v])
            if (visited1[u]) {
                dp[u] += dp[v];
                if (dp[u] >= 1e9) {
                    zero = true;
                    dp[u] %= (int) 1e9;
                }
                if (!--indegree[u]) q.emplace(u);
            }
    }

    if (count) cout << "inf";
    else if (!zero) cout << dp[2];
    else cout << setfill('0') << setw(9) << dp[2];
}
