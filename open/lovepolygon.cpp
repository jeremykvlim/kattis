#include <bits/stdc++.h>
using namespace std;

struct DisjointSet {
    vector<int> sets, size;

    int find(int p) {
        return (sets[p] == p) ? p : (sets[p] = find(sets[p]));
    }

    bool unite(int p, int q) {
        int p_set = find(p), q_set = find(q);
        if (p_set != q_set) {
            sets[q_set] = p_set;
            size[p_set] += size[q_set];
            return true;
        }
        return false;
    }

    DisjointSet(int n) : sets(n), size(n, 1) {
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

    DisjointSet dsu(n);
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
