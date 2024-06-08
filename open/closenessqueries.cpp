#include <bits/stdc++.h>
using namespace std;

struct DisjointSet {
    vector<int> sets;

    int find(int p) {
        return (sets[p] == p) ? p : (sets[p] = find(sets[p]));
    }

    bool unite(int p, int q) {
        int p_set = find(p), q_set = find(q);
        if (p_set != q_set) {
            sets[q_set] = p_set;
            return true;
        }
        return false;
    }

    DisjointSet(int n) : sets(n) {
        iota(sets.begin(), sets.end(), 0);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    DisjointSet dsu(n);
    vector<vector<int>> adj_list(n);
    while (m--) {
        int u, v;
        cin >> u >> v;

        adj_list[u].emplace_back(v);
        adj_list[v].emplace_back(u);
        dsu.unite(u, v);
    }

    int q;
    cin >> q;

    while (q--) {
        int u, v;
        cin >> u >> v;

        if (dsu.find(u) != dsu.find(v)) {
            cout << -1 << "\n";
            continue;
        }

        vector<bool> visited_u(n, false), visited_v(n, false);
        visited_u[u] = visited_v[v] = true;
        queue<int> q_u, q_v;
        q_u.emplace(u);
        q_v.emplace(v);
        for (int dist = 1;; dist++) {
            queue<int> temp;
            while (!q_u.empty()) {
                int u = q_u.front();
                q_u.pop();
                for (int v : adj_list[u]) {
                    if (visited_v[v]) {
                        cout << dist << "\n";
                        goto next;
                    }

                    if (!visited_u[v]) {
                        visited_u[v] = true;
                        temp.emplace(v);
                    }
                }
            }

            if (temp.empty()) {
                cout << "-1\n";
                next:;
                break;
            }

            visited_u.swap(visited_v);
            q_u.swap(q_v);
            q_v.swap(temp);
        }
    }
}
