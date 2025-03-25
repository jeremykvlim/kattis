#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
    vector<int> sets, size;

    int find(int v) {
        return sets[v] == v ? v : (sets[v] = find(sets[v]));
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set != v_set) {
            sets[v_set] = u_set;
            size[u_set] += size[v_set];
            return true;
        }
        return false;
    }

    DisjointSets(int n) : sets(n), size(n, 1) {
        iota(sets.begin(), sets.end(), 0);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    if (n & 1) {
        cout << "-1";
        exit(0);
    }

    vector<pair<string, string>> names(n);
    unordered_map<string, int> indices;
    for (int i = 0; i < n; i++) {
        cin >> names[i].first >> names[i].second;

        indices[names[i].first] = i;
    }

    vector<int> lover(n), indegree(n, 0);
    for (int i = 0; i < n; i++) {
        lover[i] = indices[names[i].second];
        indegree[lover[i]]++;
    }

    vector<bool> cycle(n, false);
    for (int i = 0; i < n; i++)
        if (!cycle[i] && lover[i] != i && lover[lover[i]] == i) {
            cycle[lover[i]] = cycle[i] = true;
            indegree[lover[i]]--;
            indegree[i]--;
        }

    queue<int> q;
    for (int i = 0; i < n; i++)
        if (!indegree[i]) q.emplace(i);

    int arrows = 0;
    while (!q.empty()) {
        auto v = q.front();
        q.pop();

        if (cycle[v] || cycle[lover[v]]) continue;

        cycle[v] = cycle[lover[v]] = true;
        arrows++;
        if (!cycle[lover[lover[v]]])
            if (!--indegree[lover[lover[v]]]) q.emplace(lover[lover[v]]);
    }

    DisjointSets dsu(n);
    vector<int> sets(n), size(n, 1);
    iota(sets.begin(), sets.end(), 0);
    for (int i = 0; i < n; i++)
        if (!cycle[i] && !cycle[lover[i]]) dsu.unite(i, lover[i]);

    vector<bool> visited(n, false);
    for (int i = 0, i_set; i < n; i++)
        if (!cycle[i] && !visited[i_set = dsu.find(i)]) {
            visited[i_set] = true;
            arrows += (dsu.size[i_set] + 1) / 2;
        }

    cout << arrows;
}
