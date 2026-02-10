#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    long long T;
    cin >> n >> m >> T;

    vector<vector<pair<int, int>>> adj_list(n);
    for (int _ = 0; _ < n - 1; _++) {
        int a, b, t;
        cin >> a >> b >> t;

        adj_list[a].emplace_back(b, t);
        adj_list[b].emplace_back(a, t);
    }

    vector<int> e(n - 1);
    for (int &ei : e) cin >> ei;
    e.insert(e.begin(), 0);

    vector<bool> rat(n, false);
    while (m--) {
        int r;
        cin >> r;

        rat[r] = true;
    }

    vector<long long> time(n, 0);
    vector<int> parent(n, -1), order;
    vector<vector<int>> children(n);
    auto dfs = [&](auto &&self, int v = 0) -> void {
        order.emplace_back(v);
        for (auto [u, w] : adj_list[v])
            if (u != parent[v]) {
                parent[u] = v;
                time[u] = time[v] + w;
                children[v].emplace_back(u);
                self(self, u);
            }
    };
    dfs(dfs);

    reverse(order.begin(), order.end());
    vector<long long> dp1(n, 0), dp2(n, 0);
    vector<map<long long, long long>> events(n);
    for (int v : order) {
        if (!v) break;

        if (time[v] >= T) {
            for (int u : children[v]) events[u].clear();
            events[v].clear();
            continue;
        }

        if (rat[v]) {
            for (int u : children[v]) events[u].clear();
            events[v].clear();
            events[v][time[v] + 1] = dp1[v] = e[v];
            dp2[v] = e[v] * (time[v] + 1);
            continue;
        }

        int s = 0, w = -1;
        for (int u : children[v])
            if (dp1[u]) {
                if (s < events[u].size()) {
                    s = events[u].size();
                    w = u;
                }
            }

        events[v].clear();
        if (~w) {
            events[v].swap(events[w]);
            dp1[v] = dp1[w];
            dp2[v] = dp2[w];
            events[w].clear();
        }

        for (int u : children[v])
            if (u != w) {
                if (dp1[u])
                    for (auto [t, energy] : events[u]) {
                        auto it = events[v].try_emplace(t, 0).first;
                        it->second += energy;
                        dp1[v] += energy;
                        dp2[v] += energy * t;
                    }
                    events[u].clear();
                }

        while (dp1[v] > e[v]) {
            auto it = prev(events[v].end());
            auto [t, energy] = *it;
            auto excess = dp1[v] - e[v];
            if (energy <= excess) {
                dp1[v] -= energy;
                dp2[v] -= energy * t;
                events[v].erase(it);
            } else {
                it->second = energy - excess;
                dp1[v] -= excess;
                dp2[v] -= excess * t;
                break;
            }
        }
    }

    auto energy = 0LL;
    for (int v : children[0])
        if (dp1[v]) energy += dp1[v] * (T + 1) - dp2[v];
    cout << energy;
}
