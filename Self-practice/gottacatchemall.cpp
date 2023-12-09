#include<bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<int, int>> junctions(n - 1);
    vector<vector<int>> adj_list(n + 1), optimal(n + 1);
    vector<bool> visited(n + 1, false);
    for (auto &j : junctions) {
        cin >> j.first >> j.second;
        if (j.first > j.second) swap(j.first, j.second);
    }

    sort(junctions.begin(), junctions.end());
    junctions.erase(unique(junctions.begin(), junctions.end()), junctions.end());

    for (auto &j : junctions) {
        adj_list[j.first].emplace_back(j.second);
        adj_list[j.second].emplace_back(j.first);
        int u = (adj_list[j.first].size() > adj_list[j.second].size()) ? j.first : j.second, v = j.first ^ j.second ^ u;
        optimal[u].emplace_back(v);
    }

    long long total = 0;
    for (int u = 1; u <= n; u++) {
        total += (long long) adj_list[u].size() * (adj_list[u].size() - 1) / 2;
        if (!optimal[u].empty()) {
            for (int &v : adj_list[u]) visited[v] = true;
            for (int v : optimal[u])
                for (int &w : adj_list[v]) if (visited[w]) total--;
            for (int &v : adj_list[u]) visited[v] = false;
        }
    }

    cout << (total << 1);
}
