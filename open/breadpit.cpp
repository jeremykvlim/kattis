#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<vector<int>> adj_list(n);
    for (int i = 1; i < n; i++) {
        int j;
        cin >> j;

        adj_list[j].emplace_back(i);
    }

    for (int i = n - 1; ~i; i--)
        for (auto &j : adj_list[i])
            if (adj_list[j].size() == 1) j = adj_list[j][0];

    vector<int> count(n, 0);
    while (q--) {
        int v = 0;
        while (!adj_list[v].empty()) v = adj_list[v][count[v]++ % adj_list[v].size()];
        cout << v << "\n";
    }
}
