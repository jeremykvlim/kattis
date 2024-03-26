#include <bits/stdc++.h>
using namespace std;

void dfs(int v, string &parties, vector<int> &degree, vector<vector<int>> &adj_list) {
    for (int u : adj_list[v]) {
        degree[v] += (parties[u] == parties[v]) ? -1 : 1;
        degree[u] += (parties[u] == parties[v]) ? -1 : 1;
    }

    parties[v] ^= 'A' ^ 'B';
    for (int u : adj_list[v])
        if (degree[u] > 2) dfs(u, parties, degree, adj_list);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    string parties(n, 'A');
    vector<int> degree(n);
    vector<vector<int>> adj_list(n);
    for (int i = 0; i < 5; i++) {
        int p;
        cin >> p;

        while (p--) {
            int k, l;
            cin >> k >> l;

            adj_list[k - 1].emplace_back(l - 1);
            adj_list[l - 1].emplace_back(k - 1);
            degree[k - 1]++;
            degree[l - 1]++;
        }
    }

    for (int v = 0; v < n; v++)
        if (degree[v] > 2) dfs(v, parties, degree, adj_list);

    cout << parties;
}
