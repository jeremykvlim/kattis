#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
    vector<int> sets;

    int find(int v) {
        while (sets[v] >= 0) {
            int p = sets[v];
            if (sets[p] >= 0) sets[v] = sets[p];
            v = p;
        }
        return v;
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set == v_set) return false;

        if (sets[u_set] > sets[v_set]) swap(u_set, v_set);
        sets[u_set] += sets[v_set];
        sets[v_set] = u_set;
        return true;
    }

    int size(int v) {
        return -sets[find(v)];
    }

    DisjointSets(int n) : sets(n, -1) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    DisjointSets dsu(n);
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
