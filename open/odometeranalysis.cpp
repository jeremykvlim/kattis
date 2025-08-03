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

    vector<array<int, 3>> entries(e);
    for (auto &[y, m, o] : entries) cin >> y >> m >> o;

    if (entries[0][2] != 0) {
        cout << "tampered odometer";
        exit(0);
    }

    auto diff = [&](int i, int j) -> int {
        return 12 * entries[j][0] + entries[j][1] - 12 * entries[i][0] - entries[i][1];
    };

    auto legit = [&](bool service) -> bool {
        vector<array<int, 3>> edges{{e, 0, 0}, {0, e, 0}};

        for (int i = 0; i < e; i++)
            for (int j = i + 1; j < e; j++) {
                int d = diff(i, j);
                if (d < 0) return false;

                int o = entries[j][2] - entries[i][2],
                    l = (!d ? 0 : (d - 1) * 2e3) - o, r = (!d ? 2e4 : (d + 1) * 2e4) - o,
                    wl = max(0, (int) (l + 1e5 - 1) / (int) 1e5), wr = r >= 0 ? r / 1e5 : (r - 1e5 + 1) / 1e5;
                if (wl > wr) return false;

                edges.push_back({i, j, wr});
                edges.push_back({j, i, -wl});
            }

        if (service)
            for (int i = 0; i + 1 < e; i++) {
                int d = diff(i, i + 1);
                if (d > 12) return false;

                int o = 3e4 - (entries[i + 1][2] - entries[i][2]);
                edges.push_back({i, i + 1, o >= 0 ? o / (int) 1e5 : (int) (o - 1e5 + 1) / (int) 1e5});
            }

        auto bellman_ford = [&]() -> bool {
            vector<int> dist(e + 1, 0);
            for (int _ = 0; _ < e; _++) {
                bool changed = false;
                for (auto [u, v, w] : edges)
                    if (dist[v] > dist[u] + w) {
                        dist[v] = dist[u] + w;
                        changed = true;
                    }
                if (!changed) break;
            }

            for (auto [u, v, w] : edges)
                if (dist[v] > dist[u] + w) return false;
            return true;
        };
        return bellman_ford();
    };

    auto check = [&](bool service) {
        if (!legit(service)) {
            cout << (service ? "insufficient service" : "tampered odometer");
            exit(0);
        }
    };
    check(false);
    check(true);

    cout << "seems legit";
}
