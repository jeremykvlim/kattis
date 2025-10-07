#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, xs, ys, xt, yt;
    cin >> n >> xs >> ys >> xt >> yt;

    int s = -1, t = -1;
    vector<pair<int, int>> coords(n);
    unordered_map<int, vector<pair<int, int>>> Y, X;
    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;

        if (x == xs && y == ys) s = i;
        if (x == xt && y == yt) t = i;
        coords[i] = {x, y};
        Y[x].emplace_back(y, i);
        X[y].emplace_back(x, i);
    }

    vector<int> up(n, -1), down(n, -1), left(n, -1), right(n, -1);
    for (auto &[x, y] : Y) {
        sort(y.begin(), y.end());
        for (int i = 0; i < y.size(); i++) {
            int j = y[i].second;
            if (i) down[j] = y[i - 1].second;
            if (i < y.size() - 1) up[j] = y[i + 1].second;
        }
    }

    for (auto &[y, v] : X) {
        sort(v.begin(), v.end());
        for (int i = 0; i < v.size(); i++) {
            int j = v[i].second;
            if (i) left[j] = v[i - 1].second;
            if (i < v.size() - 1) right[j] = v[i + 1].second;
        }
    }

    vector<array<int, 3>> dirs{{0, 0, 2},
                               {0, 0, 3},
                               {1, 1, 2},
                               {1, 1, 3},
                               {2, 2, 0},
                               {2, 2, 1},
                               {3, 3, 0},
                               {3, 3, 1}};

    vector<int> dist(n, -1);
    dist[s] = 0;
    queue<int> q;
    q.emplace(s);
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        if (v == t) {
            cout << dist[v];
            exit(0);
        }

        for (auto d : dirs) {
            auto next = [&](int v, array<int, 3> d) {
                auto move = [&](int v, int dir) {
                    if (!dir) return up[v];
                    if (dir == 1) return down[v];
                    if (dir == 2) return left[v];
                    return right[v];
                };

                v = move(v, d[0]);
                if (v == -1) return v;
                v = move(v, d[1]);
                if (v == -1) return v;
                return move(v, d[2]);
            };

            int u = next(v, d);
            if (u != -1 && dist[u] == -1) {
                q.emplace(u);
                dist[u] = dist[v] + 1;
            }
        }
    }
    cout << dist[t];
}
