#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, m, K;
    cin >> N >> m >> K;

    vector<vector<int>> adj_list(N + K + 1);
    while (m--) {
        int n, k;
        char c;
        cin >> n >> k >> c;

        adj_list[c == 'A' ? n : N + k].emplace_back(c == 'A' ? N + k : n);
    }

    vector<int> order, state(N + K + 1, 0);
    auto dfs = [&](auto &&self, int v) -> bool {
        state[v] = 1;
        for (int u : adj_list[v])
            if (!state[u]) {
                if (self(self, u)) return true;
            } else if (state[u] == 1) return true;

        state[v] = 2;
        order.emplace_back(v);
        return false;
    };

    for (int v = 1; v <= N + K; v++)
        if (!state[v] && dfs(dfs, v)) {
            cout << -1;
            exit(0);
        }

    reverse(order.begin(), order.end());
    for (int p : order)
        if (p <= N) cout << p << " ";
}
