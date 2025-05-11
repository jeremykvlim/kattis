#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> adj_list(n);
    vector<vector<double>> flow(n);
    vector<vector<bool>> superpower(n);
    for (int i = 0; i < n - 1; i++) {
        int a, b, x;
        bool t;
        cin >> a >> b >> x >> t;

        adj_list[a - 1].emplace_back(b - 1);
        flow[a - 1].emplace_back((double) x / 100);
        superpower[a - 1].emplace_back(t);
    }

    vector<double> liquid(n);
    for (auto &k : liquid) {
        cin >> k;

        if (k > 0) k = log(k);
    }

    double l = 0, r = 1e13, m;
    while (l + 1e-13 < r && l + l * 1e-13 < r) {
        m = l + (r - l) / 2;

        auto dfs = [&](auto &&self, double l, int v = 0) -> bool {
            if (adj_list[v].empty() && liquid[v] != -1 && l < liquid[v]) return false;

            for (int i = 0; i < adj_list[v].size(); i++) {
                int u = adj_list[v][i];

                auto f = l + log(flow[v][i]);
                if (!self(self, superpower[v][i] && f > 0 ? 2 * f : f, u)) return false;
            }

            return true;
        };

        if (dfs(dfs, m * 1e-9)) r = m;
        else l = m;
    }

    cout << fixed << setprecision(3) << exp(r * 1e-9);
}
