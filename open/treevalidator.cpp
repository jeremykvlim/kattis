#include <bits/stdc++.h>
using namespace std;

tuple<vector<int>, vector<int>, int> hopcroft_karp(int n, int m, const vector<pair<int, int>> &edges) {
    vector<int> adj_list(edges.size()), l(n, -1), r(m, -1), degree(n + 1, 0);
    for (auto [u, v] : edges) degree[u]++;
    for (int i = 1; i <= n; i++) degree[i] += degree[i - 1];
    for (auto [u, v] : edges) adj_list[--degree[u]] = v;

    int matches = 0;
    vector<int> src(n), prev(n);
    queue<int> q;
    for (;;) {
        fill(src.begin(), src.end(), -1);
        fill(prev.begin(), prev.end(), -1);

        for (int i = 0; i < n; i++)
            if (!~l[i]) q.emplace(src[i] = prev[i] = i);

        int temp = matches;
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            if (~l[src[v]]) continue;

            for (int j = degree[v]; j < degree[v + 1]; j++) {
                int u = adj_list[j];

                if (!~r[u]) {
                    while (~u) {
                        r[u] = v;
                        swap(l[v], u);
                        v = prev[v];
                    }

                    matches++;
                    break;
                }

                if (!~prev[r[u]]) {
                    q.emplace(u = r[u]);
                    prev[u] = v;
                    src[u] = src[v];
                }
            }
        }

        if (temp == matches) return {l, r, matches};
    }
}

struct DisjointSets {
    vector<int> sets;

    int find(int v) {
        return sets[v] == v ? v : (sets[v] = find(sets[v]));
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set != v_set) {
            sets[v_set] = u_set;
            return true;
        }
        return false;
    }

    DisjointSets(int n) : sets(n) {
        iota(sets.begin(), sets.end(), 0);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<int, int>> edges;
    vector<vector<int>> adj_list(n + 1);
    for (int i = 1; i < n; i++) {
        int s;
        cin >> s;

        while (s--) {
            int v;
            cin >> v;

            adj_list[v].emplace_back(i);
            edges.emplace_back(i, v);
        }
    }

    string assessment;
    cin >> assessment;

    auto correct = [&]() {
        auto [l, r, matches] = hopcroft_karp(n + 1, n + 1, edges);
        if (matches != n - 1) return false;

        int root = -1;
        for (int v = 1; v <= n; v++)
            if (r[v] == -1) {
                root = v;
                break;
            }
        if (root == -1) return false;

        vector<bool> visited_l(n + 1, false), visited_r(n + 1, false);
        visited_r[root] = true;
        queue<int> q;
        q.emplace(root);
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (int u : adj_list[v])
                if (l[u] != v && !visited_l[u]) {
                    visited_l[u] = true;
                    if (l[u] != -1 && !visited_r[l[u]]) {
                        visited_r[l[u]] = true;
                        q.emplace(l[u]);
                    }
                }
        }
        return count(visited_l.begin(), visited_l.end(), true) == n - 1;
    };

    if (!correct()) {
        if (assessment == "NO") cout << "WHO IS A GOOD KITTY?";
        else cout << "CORIANDER FOUND A SOLUTION BUT THERE ARE NONE";
    } else {
        if (assessment == "NO") cout << "CORIANDER FOUND NO SOLUTIONS BUT THERE ARE SOME";
        else {
            DisjointSets dsu(n + 1);
            vector<pair<int, int>> coriander(n - 1);
            for (auto &[a, b] : coriander) cin >> a >> b;

            if (!all_of(coriander.begin(), coriander.end(), [&](auto p) { return dsu.unite(p.first, p.second); })) {
                cout << "CORIANDER'S OUTPUT IS NOT ONE OF THE CORRECT TREES";
                exit(0);
            }

            edges.clear();
            vector<int> seen(n, 0);
            for (int j = 1, count = 1; j < n; j++, count++) {
                auto [a, b] = coriander[j - 1];
                for (int i : adj_list[a]) seen[i] = count;
                for (int i : adj_list[b])
                    if (seen[i] == count) edges.emplace_back(i, j);
            }

            auto [l, r, matches] = hopcroft_karp(n + 1, n + 1, edges);
            if (matches == n - 1) cout << "WHO IS A GOOD KITTY?";
            else cout << "CORIANDER'S OUTPUT IS NOT ONE OF THE CORRECT TREES";
        }
    }
}
