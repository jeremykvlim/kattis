#include <bits/stdc++.h>
using namespace std;

constexpr double EPSILON = 1e-6;

void dfs(int v, int prev, vector<double> &cost, vector<double> &dp, vector<vector<int>> &adj_list) {
    dp[v] = cost[v];
    for (int u : adj_list[v])
        if (u != prev) {
            dfs(u, v, cost, dp, adj_list);
            dp[v] += max(0.0, dp[u]);
        }
}

bool contains(double m, vector<int> &gdp, vector<int> &population, vector<double> &cost, vector<double> &dp, vector<vector<int>> &adj_list) {
    for (int i = 0; i < cost.size(); i++) cost[i] = gdp[i] - m * population[i];
    dfs(0, -1, cost, dp, adj_list);
    
    return dp[0] >= 0;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> gdp(n), population(n);
    for (int &c : gdp) cin >> c;
    for (int &k : population) cin >> k;

    vector<vector<int>> adj_list(n);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        
        adj_list[u - 1].emplace_back(v - 1);
        adj_list[v - 1].emplace_back(u - 1);
    }

    vector<double> cost(n), dp(n);
    double l = 0, r = INT_MAX, m;
    while (l + 1e-6 < r) {
        m = l + (r - l) / 2;
        if (!contains(m, gdp, population, cost, dp, adj_list)) r = m;
        else l = m;
    }

    cout << fixed << setprecision(6) << m;
}
