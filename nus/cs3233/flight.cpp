#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> adj_list(n + 1);
    for (int i = 0; i < n - 1; i++) {
        int a, b;
        cin >> a >> b;

        adj_list[a].emplace_back(b);
        adj_list[b].emplace_back(a);
    }

    vector<int> next(n + 1);
    iota(next.begin(), next.end(), 0);
    pair<int, int> flight{-1, -1}, cancel, add;

    auto dfs = [&](auto &&self, int v = 1, int prev = -1) -> int {
        int depth = 0;
        for (int u : adj_list[v])
            if (u != prev && make_pair(v, u) != flight && make_pair(u, v) != flight) {
                int d = self(self, u, v) + 1;

                if (depth < d) {
                    depth = d;
                    next[v] = u;
                }
            }

        return depth;
    };
    dfs(dfs);

    int least = INT_MAX;
    for (int a = 1; a <= n; a++)
        for (int b : adj_list[a])
            if (b > a) {
                flight = {a, b};
                
                auto city = [&](int v, int dist) -> int {
                    for (; dist; dist--) v = next[v];
                    return v;
                };
                
                int furthest1 = city(a, dfs(dfs, a)),
                    furthest2 = city(b, dfs(dfs, b)),
                    dist1 = dfs(dfs, furthest1),
                    dist2 = dfs(dfs, furthest2),
                    curr = max(max(dist1, dist2), (dist1 + 1) / 2 + (dist2 + 1) / 2 + 1);

                if (least > curr) {
                    least = curr;
                    cancel = flight;
                    add = {city(furthest1, dist1 / 2), city(furthest2, dist2 / 2)};
                }
            }

    cout << least << "\n" << cancel.first << " " << cancel.second << "\n" << add.first << " " << add.second << "\n";
}
