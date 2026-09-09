#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int e;
    cin >> e;

    if (e == 1) {
        cout << "seems legit";
        exit(0);
    }

    vector<int> month(e), o(e);
    for (int i = 0; i < e; i++) {
        int y, m;
        cin >> y >> m >> o[i];

        month[i] = 12 * y + m;
    }

    auto check = [&](bool service) {
        auto floor_div = [](int x, int y) {
            return x >= 0 ? x / y : -(-x + y - 1) / y;
        };

        auto ceil_div = [&](int x, int y) {
            return -floor_div(-x, y);
        };

        vector<vector<pair<int, int>>> adj_list(e);
        for (int i = 0; i < e; i++)
            for (int j = i + 1; j < e; j++) {
                int d = month[j] - month[i];
                if (d < 0) return false;

                int l = 2000 * max(d - 1, 0), r = 20000 * (d + 1);
                if (service && i + 1 == j) {
                    if (d > 12) return false;
                    r = min(r, 30000);
                }

                int wl = ceil_div(l - o[j] + o[i], 100000), wr = floor_div(r - o[j] + o[i], 100000);
                if (wl > wr) return false;

                adj_list[i].emplace_back(j, wr);
                adj_list[j].emplace_back(i, -wl);
            }

        vector<int> dist(e, 0), len(e, 0);
        vector<bool> queued(e, true);
        deque<int> dq;
        for (int i = 0; i < e; i++) dq.emplace_back(i);

        while (!dq.empty()) {
            int v = dq.front();
            dq.pop_front();

            queued[v] = false;

            for (auto [u, w] : adj_list[v])
                if (dist[u] > dist[v] + w) {
                    dist[u] = dist[v] + w;
                    len[u] = len[v] + 1;

                    if (len[u] >= e) return false;

                    if (!queued[u]) {
                        queued[u] = true;
                        if (dq.empty() || dist[u] < dist[dq.front()]) dq.emplace_front(u);
                        else dq.emplace_back(u);
                    }
                }
        }
        return true;
    };

    if (!check(false)) cout << "tampered odometer";
    else if (!check(true)) cout << "insufficient service";
    else cout << "seems legit";
}