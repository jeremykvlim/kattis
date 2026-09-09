#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n && ~n) {
        vector<vector<pair<int, int>>> adj_list_regular(n + 1), adj_list_transpose(n + 1);
        for (int u = 1; u <= n; u++) {
            int e, doorways;
            cin >> e >> doorways;

            while (doorways--) {
                int v;
                cin >> v;

                adj_list_regular[u].emplace_back(v, e);
                adj_list_transpose[v].emplace_back(u, e);
            }
        }

        vector<bool> reachable(n + 1, false);
        reachable[n] = true;
        deque<int> dq;
        dq.emplace_back(n);
        while (!dq.empty()) {
            int v = dq.front();
            dq.pop_front();

            for (auto [u, w] : adj_list_transpose[v])
                if (!reachable[u]) {
                    reachable[u] = true;
                    dq.emplace_back(u);
                }
        }

        vector<int> dist(n + 1, INT_MIN), len(n + 1, 0);
        dist[1] = 100;
        vector<bool> queued(n + 1, false);
        queued[1] = true;
        dq.emplace_back(1);
        while (!dq.empty()) {
            int v = dq.front();
            dq.pop_front();

            queued[v] = false;

            for (auto [u, w] : adj_list_regular[v])
                if (reachable[u] && max(0, dist[u]) < dist[v] + w) {
                    dist[u] = dist[v] + w;
                    len[u] = len[v] + 1;

                    if (u == n || len[u] == n) {
                        cout << "winnable\n";
                        goto next;
                    }

                    if (!queued[u]) {
                        queued[u] = true;
                        if (dq.empty() || dist[u] > dist[dq.front()]) dq.emplace_front(u);
                        else dq.emplace_back(u);
                    }
                }
        }

        cout << "hopeless\n";
        next:;
    }
}