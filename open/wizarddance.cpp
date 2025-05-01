#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> p(n), cw(n), ccw(n), degree(n, 0);
    vector<vector<pair<int, int>>> adj_list(n);
    for (int i = 0; i < n; i++) {
        cin >> p[i];

        int l = (i + p[i]) % n, r = (i - p[i] + n) % n;
        cw[i] = l;
        ccw[i] = r;
        if (l != r) {
            adj_list[l].emplace_back(r, i);
            adj_list[r].emplace_back(l, i);
            degree[l]++;
            degree[r]++;
        } else {
            adj_list[l].emplace_back(l, i);
            degree[l]++;
        }
    }

    queue<int> q;
    for (int i = 0; i < n; i++)
        if (degree[i] == 1) q.emplace(i);

    vector<int> teleport(n, -1);
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        if (degree[v] <= 0) continue;

        for (auto [u, i] : adj_list[v]) {
            if (degree[u] <= 0) continue;

            teleport[i] = v;
            if (--degree[u] == 1) q.emplace(u);
            break;
        }
        degree[v]--;
    }

    for (int i = 0; i < n; i++)
        if (!~teleport[i]) {
            int l = cw[i], r = ccw[i];
            if (degree[l] > 0) q.emplace(l);
            else if (degree[r] > 0) q.emplace(r);
        }

    while (!q.empty()) {
        int v = q.front();
        q.pop();

        if (degree[v]) 
            for (int u = v;;) {
                auto it = find_if(adj_list[u].begin(), adj_list[u].end(), [&](auto p) { return !~teleport[p.second]; });
                if (it == adj_list[u].end()) break;

                auto [t, i] = *it;
                teleport[i] = u;
                degree[u] = 0;

                if (u == t || v == t) break;
                u = t;
            }
    }

    if (any_of(teleport.begin(), teleport.end(), [&](int p) { return !~p; })) {
        cout << "no dance";
        exit(0);
    }

    for (int i = 0; i < n; i++) cout << (teleport[i] == cw[i] ? 'L' : 'R');
}
