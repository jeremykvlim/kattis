#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int w, h;
    while (cin >> w >> h && w && h) {
        vector<vector<pair<int, int>>> adj_list(w * h);
        vector<vector<int>> state(w, vector<int>(h, 1));
        state[0][0] = 0;
        state[w - 1][h - 1] = -1;

        int g;
        cin >> g;

        while (g--) {
            int x, y;
            cin >> x >> y;

            state[x][y] = 2;
        }

        int e;
        cin >> e;

        while (e--) {
            int x1, y1, x2, y2, t;
            cin >> x1 >> y1 >> x2 >> y2 >> t;

            state[x1][y1] = -1;
            adj_list[x1 * h + y1].emplace_back(x2 * h + y2, t);
        }

        vector<int> dx{1, 0, -1, 0}, dy{0, 1, 0, -1};
        for (int i = 0; i < w; i++)
            for (int j = 0; j < h; j++)
                if (!state[i][j] || state[i][j] == 1)
                    for (int k = 0; k < 4; k++) {
                        int x = i + dx[k], y = j + dy[k];
                        if (0 <= x && x < w && 0 <= y && y < h && state[x][y] != 2)
                            adj_list[i * h + j].emplace_back(x * h + y, 1);
                    }

        vector<int> dist(w * h, INT_MAX), count(w * h, 0);
        dist[0] = 0;
        deque<int> dq{0};
        vector<bool> queued(w * h, false);
        queued[0] = true;
        while (!dq.empty()) {
            int v = dq.front();
            dq.pop_front();
            
            queued[v] = false;

            for (auto [u, d] : adj_list[v])
                if (dist[u] > dist[v] + d) {
                    dist[u] = dist[v] + d;
                    count[u]++;

                    if (!u || count[u] == w * h) {
                        cout << "Never\n";
                        goto next;
                    }

                    if (!queued[u]) {
                        if (dq.empty() || dist[u] < dist[dq.front()]) dq.emplace_front(u);
                        else dq.emplace_back(u);
                        queued[u] = true;
                    }
                }
        }

        cout << (dist.back() == INT_MAX ? "Impossible\n" : to_string(dist.back()) + "\n");
        next:;
    }
}
