#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<set<pair<int, int>>> adj_list(2 * n);
    vector<int> leaves;

    for (int e = 0; e < 2 * (n - 1); ++e) {
        int u, v;
        cin >> u >> v;
        adj_list[v].insert({n + u, e});
        adj_list[n + u].insert({v, e});
    }

    for (int u = 2; u < 2 * n; ++u) {
        if (adj_list[u].empty()) {
            cout << "impossible";
            return 0;
        }
        if (adj_list[u].size() == 1) leaves.push_back(u);
    }

    string solution(2*(n - 1), ' ');

    while (!leaves.empty()) {
        int u = leaves.back();
        leaves.pop_back();
        if (adj_list[u].empty()) {
            cout << "impossible";
            return 0;
        }
        auto [v, e] = *adj_list[u].begin();
        adj_list[u].erase(adj_list[u].begin());
        adj_list[v].erase({u, e});
        solution[e] = u <= n ? 'L' : 'R';
        if (adj_list[v].size() == 1) leaves.push_back(v);
    }

    for (int u = 2; u < 2 * n; ++u)
        while (!adj_list[u].empty()) {
            auto [v, e] = *adj_list[u].begin();
            adj_list[u].erase(adj_list[u].begin());
            adj_list[v].erase({u, e});
            solution[e] = u <= n ? 'L' : 'R';
            u = v;
        }

    cout << solution;
}
