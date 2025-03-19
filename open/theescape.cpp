#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int V, P;
    cin >> V >> P;

    vector<vector<int>> adj_list(V);
    for(int i = 0; i < V - 1; i++) {
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

    vector<int> dp1(V), dp2(V), dp3(V);
    auto dfs = [&](auto &&self, int v, int prev = -1) {
        if (v != start && adj_list[v].size() == 1) {
            dp1[v] = 0;
            dp2[v] = dp3[v] = !pig[v];
            return;
        }

        bool done = false;
        for (int u : adj_list[v])
            if (u != prev) {
                self(self, u, v);

                if (!done) {
                    if (pig[v]) {
                        dp1[v] = dp3[u];
                        dp2[v] = min(dp1[u], dp2[u]);
                        dp3[v] = dp3[u];
                    } else {
                        dp1[v] = dp1[u];
                        dp2[v] = min(dp1[u], dp2[u]) + 1;
                        dp3[v] = dp3[u] + 1;
                    }
                    done = true;
                } else {
                    int c1 = min(dp1[v] + dp1[u], min(dp2[v] + dp3[u], dp2[u] + dp3[v])),
                        c2 = dp2[v] + min(dp1[u], dp2[u]),
                        c3 = min(dp3[v] + min(dp1[u], dp2[u]), dp2[v] + dp3[u]);

                    dp1[v] = c1;
                    dp2[v] = c2;
                    dp3[v] = c3;
                }
            }
    };
    dfs(dfs, start);
    cout << dp1[start];
}
