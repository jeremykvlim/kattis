#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k, T;
    cin >> n >> m >> k >> T;

    vector<array<int, 3>> edges(m);
    vector<vector<int>> on(n + 1), edge_id(n + 1, vector<int>(n + 1, -1));
    for (int e = 0; e < m; e++) {
        auto &[u, v, w] = edges[e];
        cin >> u >> v;
        w = 1;

        edge_id[u][v] = e;
        on[u].emplace_back(e);
    }

    vector<vector<vector<int>>> forbidden(m);
    while (T--) {
        int l;
        cin >> l;

        vector<int> u(l);
        for (int &ui : u) cin >> ui;

        if (l > k) continue;

        int e = edge_id[u[0]][u[1 % l]], pos = 0;
        for (int i = 1; i < l; i++)
            if (e > edge_id[u[i]][u[(i + 1) % l]]) {
                e = edge_id[u[i]][u[(i + 1) % l]];
                pos = i;
            }

        vector<int> path;
        for (int i = 1; i < l; i++) path.emplace_back(edge_id[u[(pos + i) % l]][u[(pos + i + 1) % l]]);
        forbidden[e].emplace_back(path);
    }

    vector<bool> mask(m, true);
    for (int e = 0; e < m; e++) {
        mask[e] = false;
        int s = edges[e][1], t = edges[e][0];
        auto optimize = [&](const auto &state) -> vector<int> {
            if (s == t) return {};

            vector<int> dist(n + 1, -1), prev(n + 1, -1);
            dist[s] = 0;
            queue<int> q;
            q.emplace(s);
            while (!q.empty()) {
                int v = q.front();
                q.pop();

                if (dist[v] == k - 1) continue;

                for (int e : on[v]) {
                    if (!state.first[e]) continue;
                    auto [_, u, w] = edges[e];
                    if (!~dist[u]) {
                        dist[u] = dist[v] + 1;
                        q.emplace(u);
                        prev[u] = e;
                    }
                }
            }
            if (prev[t] == -1) return {};

            vector<int> path;
            for (int v = t; v != s; v = edges[path.back()][0]) path.emplace_back(prev[v]);
            reverse(path.begin(), path.end());
            return path;
        };

        auto partition = [&](const auto &state, const auto &solution) {
            vector<pair<vector<bool>, int>> substates;
            auto [m, i] = state;
            for (; i < solution.size(); i++) {
                m[solution[i]] = false;
                substates.push_back({m, i});

                int v = edges[solution[i]][0];
                for (int e : on[v]) m[e] = false;
                m[solution[i]] = true;
            }
            return substates;
        };

        priority_queue<tuple<int, pair<vector<bool>, int>, vector<int>>> pq;
        auto enqueue = [&](const auto &state) {
            auto solution = optimize(state);
            if (!solution.empty()) pq.emplace(-solution.size(), state, solution);
        };
        enqueue(make_pair(mask, 0));

        while (!pq.empty()) {
            auto [w, state, solution] = pq.top();
            pq.pop();

            if (find(forbidden[e].begin(), forbidden[e].end(), solution) == forbidden[e].end()) {
                cout << "Incomplete";
                exit(0);
            }

            for (auto &ss : partition(state, solution)) enqueue(ss);
        }
    }
    cout << "Complete";
}
