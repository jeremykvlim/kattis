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

        adj_list[u].emplace_back(v);
        adj_list[v].emplace_back(u);
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

        vector<bool> visited_u(n, false), visited_v(n, false);
        visited_u[u] = true;
        visited_v[v] = true;
        queue<int> q_u, q_v;
        q_u.emplace(u);
        q_v.emplace(v);
        int dist = 0;
        for (;;) {
            dist++;
            queue<int> temp;
            while (!q_u.empty()) {
                int u = q_u.front();
                q_u.pop();
                for (auto v : adj_list[u]) {
                    if (visited_v[v]) {
                        cout << dist << '\n';
                        goto next;
                    }

                    if (!visited_u[v]) {
                        visited_u[v] = true;
                        temp.emplace(v);
                    }
                }
            }

            if (temp.empty()) {
                cout << -1 << '\n';
                next:;
                break;
            }

            visited_u.swap(visited_v);
            q_u.swap(q_v);
            q_v.swap(temp);
        }
    }
}
