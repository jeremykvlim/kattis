#include <bits/stdc++.h>
using namespace std;

void dfs(vector<vector<int>> &adj_list, vector<int> &order, vector<int> &prev, int v) {
    for (int u : adj_list[v]) {
        if (u == prev[v]) continue;
        
        prev[u] = v;
        order.emplace_back(u);
        dfs(adj_list, order, prev, u);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    if (!(n & 1)) {
        cout << "IMPOSSIBLE";
        exit(0);
    }

    vector<vector<int>> adj_list(n);
    for (int i = 0; i < n - 1; i++) {
        int a, b;
        cin >> a >> b;

        adj_list[a - 1].emplace_back(b - 1);
        adj_list[b - 1].emplace_back(a - 1);
    }

    vector<int> order, prev(n, -1);
    dfs(adj_list, order, prev, 0);

    vector<pair<int, int>> big, sugar;
    for (int i = 0; i < order.size(); i += 2) {
        big.emplace_back(order[i], prev[order[i]]);
        sugar.emplace_back(order[i + 1], prev[order[i + 1]]);
        if (order[i] == prev[order[i + 1]]) {
            big.emplace_back(prev[order[i]], order[i + 1]);
            sugar.emplace_back(prev[order[i]], order[i + 1]);
        } else {
            big.emplace_back(order[i], order[i + 1]);
            sugar.emplace_back(order[i], order[i + 1]);
        }
    }

    for (auto [a, b] : big) cout << a + 1 << " " << b + 1 << "\n";
    for (auto [a, b] : sugar) cout << a + 1 << " " << b + 1 << "\n";
}
