#include <bits/stdc++.h>
using namespace std;

int find(int p, vector<int> &sets) {
    return (sets[p] == p) ? p : (sets[p] = find(sets[p], sets));
}

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

    vector<int> sets(n), size(n, 1);
    iota(sets.begin(), sets.end(), 0);
    for (int i = 0; i < n; i++)
        if (!cycle[i] && !cycle[lover[i]]) {
            int i_set = find(i, sets), lover_set = find(lover[i], sets);
            if (i_set != lover_set) {
                sets[lover_set] = i_set;
                size[i_set] += size[lover_set];
            }
        }

    vector<bool> visited(n, false);
    for (int i = 0, i_set; i < n; i++)
        if (!cycle[i] && !visited[i_set = find(i, sets)]) {
            visited[i_set] = true;
            arrows += (size[i_set] + 1) / 2;
        }

    cout << arrows;
}
