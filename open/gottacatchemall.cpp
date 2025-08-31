#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<int, int>> junctions(n - 1);
    for (auto &[u, v] : junctions) {
        cin >> u >> v;

        if (u > v) swap(u, v);
    }
    sort(junctions.begin(), junctions.end());
    junctions.erase(unique(junctions.begin(), junctions.end()), junctions.end());

    vector<int> count(n + 1, 0);
    for (auto [u, v] : junctions) {
        count[u]++;
        count[v]++;
    }

    vector<vector<int>> connected(n + 1);
    for (auto [u, v] : junctions) {
        if (count[u] != count[v] ? count[u] > count[v] : u < v) connected[u].emplace_back(v);
        else connected[v].emplace_back(u);
    }

    vector<int> visited(n + 1);
    auto total = 0LL, optimal = 0LL;
    for (int i = 1; i <= n; i++) {
        if (count[i]) total += (long long) count[i] * (count[i] - 1);
        for (int j : connected[i]) visited[j] = i;
        for (int j : connected[i])
            for (int k : connected[j])
                if (visited[k] == i) optimal++;
    }

    cout << total - 6 * optimal;
}
