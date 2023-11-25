#include <bits/stdc++.h>
using namespace std;

constexpr double OMEGA = 1e10;

bool dfs(double l, int v, vector<double> &k, vector<vector<int>> &adj_list, vector<vector<double>> &flow,
         vector<vector<bool>> &superpower, int parent = -1) {
    if (adj_list[v].empty() && k[v] != -1 && l < k[v]) return false;
    for (int i = 0; i < adj_list[v].size(); i++) {
        int u = adj_list[v][i];
        if (u == parent) continue;
        double f = l + log(flow[v][i]);
        if (!dfs(superpower[v][i] && f > 0 ? 2 * f : f, u, k, adj_list, flow, superpower, u)) return false;
    }
    return true;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> adj_list(n);
    vector<vector<double>> flow(n);
    vector<vector<bool>> superpower(n);

    for (int i = 0; i < n - 1; ++i) {
        int a, b, x, t;
        cin >> a >> b >> x >> t;
        adj_list[a - 1].push_back(b - 1);
        flow[a - 1].push_back(x / 100.0);
        superpower[a - 1].push_back(t);
    }

    vector<double> k(n);
    for (auto &ki : k) {
        cin >> ki;
        if (ki > 0) ki = log(ki);
    }

    double l = 0, r = OMEGA + log(LLONG_MAX) * OMEGA;
    while (l + 1 < r) {
        double m = l + (r - l) / 2;
        if (dfs(m / OMEGA, 0, k, adj_list, flow, superpower)) r = m;
        else l = m;
    }

    cout << fixed << setprecision(4) << exp(r / OMEGA) << "\n";
}
