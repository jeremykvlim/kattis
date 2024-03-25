#include <bits/stdc++.h>
using namespace std;

int dfs(int v, int prev, vector<vector<int>> &adj_list, vector<int> &next, pair<int, int> &flight) {
    int depth = 0;
    for (int u : adj_list[v])
        if (u != prev && (v != flight.first || u != flight.second) && (u != flight.first || v != flight.second)) {
            int d = dfs(u, v, adj_list, next, flight) + 1;

            if (d > depth) {
                depth = d;
                next[v] = u;
            }
        }

    return depth;
}

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
    dfs(1, -1, adj_list, next, flight);

    auto city = [&](auto &&city, int v, int dist) -> int {
        return !dist ? v : city(city, next[v], dist - 1);
    };

    int least = INT_MAX;
    for (int a = 1; a <= n; a++)
        for (int b : adj_list[a])
            if (b > a) {
                flight = {a, b};
                int furthest1 = city(city, a, dfs(a, -1, adj_list, next, flight)), furthest2 = city(city, b, dfs(b, -1, adj_list, next, flight)),
                        dist1 = dfs(furthest1, -1, adj_list, next, flight), dist2 = dfs(furthest2, -1, adj_list, next, flight),
                         curr = max(max(dist1, dist2), (dist1 + 1) / 2 + (dist2 + 1) / 2 + 1);

                if (curr < least) {
                    least = curr;
                    cancel = flight;
                    add = {city(city, furthest1, dist1 / 2), city(city, furthest2, dist2 / 2)};
                }
            }

    cout << least << "\n" << cancel.first << " " << cancel.second << "\n" << add.first << " " << add.second << "\n";
}
