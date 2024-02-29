#include <bits/stdc++.h>
using namespace std;

bool dfs(long double l, int v, vector<long double> &liquid, vector<vector<int>> &adj_list, vector<vector<long double>> &flow,
         vector<vector<bool>> &superpower, int parent = -1) {
    if (adj_list[v].empty() && liquid[v] != -1 && l < liquid[v]) return false;
         
    for (int i = 0; i < adj_list[v].size(); i++) {
        int u = adj_list[v][i];
        if (u == parent) continue;
        auto f = l + log(flow[v][i]);
        if (!dfs(superpower[v][i] && f > 0 ? 2 * f : f, u, liquid, adj_list, flow, superpower, u)) return false;
    }
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> adj_list(n);
    vector<vector<long double>> flow(n);
    vector<vector<bool>> superpower(n);
    for (int i = 0; i < n - 1; i++) {
        int a, b, x, t;
        cin >> a >> b >> x >> t;
             
        adj_list[a - 1].emplace_back(b - 1);
        flow[a - 1].emplace_back(x / 100.0);
        superpower[a - 1].emplace_back(t);
    }

    vector<long double> liquid(n);
    for (auto &k : liquid) {
        cin >> k;
        if (k > 0) k = log(k);
    }

    long double inf = INT_MAX * log(INT_MAX), l = 0, r = inf + log(LLONG_MAX) * inf, m;
    while (l + 1 < r) {
        m = l + (r - l) / 2;
        if (dfs(m / inf, 0, liquid, adj_list, flow, superpower)) r = m;
        else l = m;
    }

    cout << fixed << setprecision(3) << exp(r / inf);
}
