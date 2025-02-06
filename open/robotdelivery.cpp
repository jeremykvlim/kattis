#include <bits/stdc++.h>
using namespace std;

tuple<vector<int>, vector<int>, int> hopcroft_karp(int n, int m, vector<pair<int, int>> edges) {
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

    int R, C, n;
    cin >> R >> C >> n;

    vector<vector<char>> grid(R, vector<char>(C));
    vector<pair<int, int>> robots, people;
    for (int i = 0; i < R; i++)
        for (int j = 0; j < C; j++) {
            cin >> grid[i][j];

            if (grid[i][j] == 'R') robots.emplace_back(i, j);
            else if (grid[i][j] == 'P') people.emplace_back(i, j);
        }

    int d = -1;
    vector<int> dx{1, 0, -1, 0}, dy{0, 1, 0, -1};
    vector<vector<int>> robotToPerson(n, vector<int>(n, -1));
    queue<pair<int,int>> q;
    for (int i = 0; i < n; i++) {
        auto [sr, sc] = robots[i];
        vector<vector<int>> dist(R, vector<int>(C, -1));
        dist[sr][sc] = 0;
        q.emplace(sr, sc);
        while (!q.empty()) {
            auto [r, c] = q.front();
            q.pop();

            for (int k = 0; k < 4; k++) {
                int x = r + dx[k], y = c + dy[k];
                if (!(0 <= x && x < R && 0 <= y && y < C) || grid[x][y] == '#') continue;
                if (!~dist[x][y]) {
                    dist[x][y] = dist[r][c] + 1;
                    q.emplace(x, y);
                }
            }
        }

        for (int j = 0; j < n; j++) {
            auto [pr, pc] = people[j];
            if (dist[pr][pc] != -1) {
                robotToPerson[i][j] = dist[pr][pc];
                d = max(d, dist[pr][pc]);
            }
        }
    }

    auto matchable = [&](int k) -> bool {
        vector<pair<int, int>> edges;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (robotToPerson[i][j] != -1 && robotToPerson[i][j] <= k) edges.emplace_back(i, j);

        auto [l, r, matches] = hopcroft_karp(n, n, edges);
        return matches == n;
    };

    int l = 0, r = d, mid;
    while (l + 1 < r) {
        mid = l + (r - l) / 2;

        if (matchable(mid)) r = mid;
        else l = mid;
    }
    cout << r;
}
