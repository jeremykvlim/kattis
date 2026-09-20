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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> p(n);
    for (int &pi : p) cin >> pi;

    vector<int> cw(n), ccw(n);
    vector<pair<int, int>> edges;
    for (int i = 0; i < n; i++) {
        cw[i] = (i + p[i]) % n;
        ccw[i] = (i - p[i] + n) % n;

        edges.emplace_back(i, cw[i]);
        edges.emplace_back(i, ccw[i]);
    }
    auto [l, r, matches] = hopcroft_karp(n, n, edges);

    if (matches != n) {
        cout << "no dance";
        exit(0);
    }

    string s(n, 'L');
    vector<int> order(n);
    iota(order.begin(), order.end(), 0);
    for (int i = 0; i < n; i++)
        if (l[i] == cw[i]) order[i] = r[ccw[i]];
        else {
            s[i] = 'R';
            order[i] = r[cw[i]];
        }

    vector<int> state(n, 0);
    for (int t = 0; t < n; t++)
        if (!state[t]) {
            vector<int> path;
            int v = t;
            while (~v && !state[v]) {
                state[v] = 1;
                path.emplace_back(v);
                v = order[v];
            }

            if (~v && state[v] == 1) {
                int i = v;
                for (int u = order[v]; u != v; u = order[u]) i = min(i, u);

                if (s[i] == 'R') {
                    int u = v;
                    do {
                        s[u] = s[u] == 'L' ? 'R' : 'L';
                        u = order[u];
                    } while (u != v);
                }
            }

            for (int u : path) state[u] = 2;
        }
    cout << s;
}