#include <bits/stdc++.h>
using namespace std;

int find(int p, vector<int> &sets) {
    return (sets[p] == p) ? p : (sets[p] = find(sets[p], sets));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj_list(n);
    vector<int> sets(n), size(n, 1);
    iota(sets.begin(), sets.end(), 0);
    while (m--) {
        int u, v;
        cin >> u >> v;
        adj_list[u].push_back(v);
        adj_list[v].push_back(u);
        sets[find(v, sets)] = find(u, sets);
    }

    int q;
    cin >> q;

    while (q--) {
        int u, v;
        cin >> u >> v;

        if (find(u, sets) != find(v, sets)) {
            cout << -1 << '\n';
            continue;
        }

        unordered_set<int> visited1{u}, visited2{v};
        vector<int> q1{u}, q2{v};

        int dist = 0;
        for (;;) {
            dist++;
            vector<int> q3;
            for (auto &i : q1)
                for (auto &j : adj_list[i]) {
                    if (visited2.count(j)) {
                        cout << dist << '\n';
                        goto next;
                    }

                    if (!visited1.count(j)) {
                        visited1.insert(j);
                        q3.push_back(j);
                    }
                }

            if (q3.empty()) {
                cout << -1 << '\n';
                goto next;
            }

            visited1.swap(visited2);
            q1.swap(q2);
            q2.swap(q3);
        }
        next:;
    }
}
