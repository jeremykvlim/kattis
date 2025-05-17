#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    vector<vector<int>> adj_list(n + 1);
    while (m--) {
        int a, b;
        cin >> a >> b;

        adj_list[a].emplace_back(b);
    }

    vector<int> dist(n + 1, 1);
    for (int a = 1; a <= n; a++)
        for (int b : adj_list[a]) dist[b] = max(dist[b], dist[a] + 1);

    int Q;
    cin >> Q;

    vector<bool> visited(n + 1, false);
    while (Q--) {
        int c, d;
        cin >> c >> d;

        if (dist[c] + 1 <= dist[d]) {
            accept:;
            cout << "accept\n";
            adj_list[c].emplace_back(d);
            continue;
        }

        fill(visited.begin(), visited.end(), false);
        visited[d] = true;
        vector<pair<int, int>> undo{{d, dist[d]}};
        dist[d] = dist[c] + 1;
        if (dist[d] > k) goto reject;
        else {
            queue<int> q;
            q.emplace(d);
            while (!q.empty()) {
                int a = q.front();
                q.pop();

                for (int b : adj_list[a])
                    if (dist[a] + 1 > dist[b]) {
                        if (!visited[b]) {
                            visited[b] = true;
                            undo.emplace_back(b, dist[b]);
                        }

                        dist[b] = dist[a] + 1;
                        if (dist[b] > k) goto reject;
                        q.emplace(b);
                    }
            }
        }
        goto accept;

        reject:;
        cout << "reject\n";
        for (auto [b, di] : undo) dist[b] = di;
    }
}
