#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int V, P;
    cin >> V >> P;

    vector<vector<int>> adj_list(V);
    for (int _ = 0; _ < V - 1; _++) {
        int u, v;
        cin >> u >> v;

        adj_list[u].emplace_back(v);
        adj_list[v].emplace_back(u);
    }

    vector<bool> pig(V, false);
    while (P--) {
        int p;
        cin >> p;

        pig[p] = true;
    }

    int start = 0;
    for (int i = 0; i < V; i++)
        if (adj_list[i].size() != 1) {
            start = i;
            break;
        }

    vector<int> dp1(V, 0), dp2(V), dp3(V);
    for (int i = 0; i < V; i++) dp2[i] = dp3[i] = !pig[i];
    auto dfs = [&](auto &&self, int v, int prev = -1) {
        if (v != start && adj_list[v].size() == 1) return;

        bool done = false;
        for (int u : adj_list[v])
            if (u != prev) {
                self(self, u, v);

                if (!done) {
                    if (pig[v]) {
                        dp1[v] = dp2[v] = dp2[u];
                        dp3[v] = min(dp1[u], dp3[u]);
                    } else {
                        dp1[v] = dp1[u];
                        dp2[v] = dp2[u] + 1;
                        dp3[v] = min(dp1[u], dp3[u]) + 1;
                    }
                    done = true;
                } else {
                    dp1[v] = min(dp1[v] + dp1[u], min(dp3[v] + dp2[u], dp3[u] + dp2[v]));
                    dp2[v] = min(dp2[v] + min(dp1[u], dp3[u]), dp3[v] + dp2[u]);
                    dp3[v] += min(dp1[u], dp3[u]);
                }
            }
    };
    dfs(dfs, start);
    cout << dp1[start];
}
