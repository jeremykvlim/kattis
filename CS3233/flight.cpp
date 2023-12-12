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

int find(int v, int flights, vector<int> &next) {
    return flights ? find(next[v], flights - 1, next) : v;
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
    pair<int, int> flight = {-1, -1}, p1, p2;
    dfs(1, -1, adj_list, next, flight);

    int least = INT_MAX;
    for (int a = 1; a <= n; a++)
        for (int b : adj_list[a]) {
            if (b > a) {
                flight = {a, b};
                int furthest1 = find(a, dfs(a, -1, adj_list, next, flight), next), furthest2 = find(b, dfs(b, -1, adj_list, next, flight), next),
                        dist1 = dfs(furthest1, -1, adj_list, next, flight), dist2 = dfs(furthest2, -1, adj_list, next, flight),
                        curr = max(max(dist1, dist2), (dist1 + 1) / 2 + (dist2 + 1) / 2 + 1);
                if (curr < least) {
                    least = curr;
                    p1 = flight;
                    p2 = {find(furthest1, dist1 / 2, next), find(furthest2, dist2 / 2, next)};
                }
            }
        }
    
    cout << least << "\n" << p1.first << " " << p1.second << "\n" << p2.first << " " << p2.second << "\n";
}
