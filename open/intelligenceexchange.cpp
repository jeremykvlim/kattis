#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    while (cin >> n >> m && n && m) {
        int hc, sc, hj, sj;
        cin >> hc >> sc >> hj >> sj;

        vector<tuple<int, int, long long>> edges(m);
        vector<vector<pair<int, long long>>> adj_list(n + 1);
        for (auto &[u, v, w] : edges) {
            cin >> u >> v >> w;

            adj_list[u].emplace_back(v, w);
            adj_list[v].emplace_back(u, w);
        }

        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
        auto dijkstra = [&](int s) {
            vector<long long> dist(n + 1, 1e18);
            dist[s] = 0;
            pq.emplace(0, s);
            while (!pq.empty()) {
                auto [d, v] = pq.top();
                pq.pop();

                if (d != dist[v]) continue;

                for (auto [u, w] : adj_list[v])
                    if (dist[u] > d + w) {
                        dist[u] = d + w;
                        pq.emplace(dist[u], u);
                    }
            }
            return dist;
        };
        auto dist_hc = dijkstra(hc), dist_sc = dijkstra(sc), dist_hj = dijkstra(hj), dist_sj = dijkstra(sj);

        int r = 0;
        vector<bool> visited(n + 1, false);
        for (int v = 1; v <= n; v++)
            if (dist_hc[v] + dist_sc[v] == dist_hc[sc] && dist_hj[v] + dist_sj[v] == dist_hj[sj] && dist_hc[v] == dist_hj[v]) {
                r++;
                visited[v] = true;
            }

        if (dist_hj[sc] + dist_sj[sc] == dist_hj[sj] && dist_hj[sc] >= dist_hc[sc] && !visited[sc]) {
            r++;
            visited[sc] = true;
        }
        if (dist_hc[sj] + dist_sc[sj] == dist_hc[sc] && dist_hc[sj] >= dist_hj[sj] && !visited[sj]) {
            r++;
            visited[sj] = true;
        }

        for (auto [u, v, w] : edges) {
            bool c_uv = (dist_hc[u] + w + dist_sc[v] == dist_hc[sc]),
                 c_vu = (dist_hc[v] + w + dist_sc[u] == dist_hc[sc]),
                 j_uv = (dist_hj[u] + w + dist_sj[v] == dist_hj[sj]),
                 j_vu = (dist_hj[v] + w + dist_sj[u] == dist_hj[sj]);

            if ((c_uv && j_uv && dist_hc[u] == dist_hj[u]) || (c_vu && j_vu && dist_hc[v] == dist_hj[v])) {
                cout << "-1\n";
                goto next;
            }

            if (c_uv && j_vu && -w < dist_hj[v] - dist_hc[u] && dist_hj[v] - dist_hc[u] < w) r++;
            if (c_vu && j_uv && -w < dist_hc[v] - dist_hj[u] && dist_hc[v] - dist_hj[u] < w)
                if (!(c_uv && j_vu && dist_hj[v] - dist_hc[u] == dist_hc[v] - dist_hj[u])) r++;
        }
        cout << r << "\n";
        next:;
    }
}
