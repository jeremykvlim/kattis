#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> degree(n, 0);
    vector<pair<int, int>> edges(m);
    for (auto &[u, v] : edges) {
        cin >> u >> v;

        degree[u]++;
        degree[v]++;
    }

    int p;
    cin >> p;

    vector<double> police(n, 0);
    while (p--) {
        int w;
        cin >> w;

        police[w]++;
    }

    auto encounters = 1e20;
    vector<double> dp(n, 1e20), ratio(n), temp(n);
    dp[0] = 0;
    while (encounters == 1e20 || encounters > *min_element(dp.begin(), dp.end())) {
        for (int v = 0; v < n; v++) ratio[v] = police[v] / degree[v];

        fill(temp.begin(), temp.end(), 0);
        for (auto [u, v] : edges) {
            temp[u] += ratio[v];
            temp[v] += ratio[u];
        }
        police = temp;

        fill(temp.begin(), temp.end(), 1e20);
        for (auto [u, v] : edges) {
            temp[u] = min(temp[u], dp[v]);
            temp[v] = min(temp[v], dp[u]);
        }
        for (int v = 0; v < n; v++) temp[v] += police[v];
        dp = temp;

        encounters = min(encounters, dp[n - 1]);
        dp[n - 1] = 1e20;
    }
    cout << fixed << setprecision(6) << encounters;
}
