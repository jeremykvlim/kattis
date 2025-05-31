#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    int max_s = 0;
    vector<int> required(n);
    for (int &s : required) {
        int vi;
        cin >> vi;

        s = (vi - 20) / 2;
        max_s = max(max_s, s);
    }

    unordered_map<int, int> w;
    while (k--) {
        int wi;
        cin >> wi;

        w[wi]++;
    }
    vector<pair<int, int>> deduped{w.begin(), w.end()};
    sort(deduped.rbegin(), deduped.rend());
    int m = deduped.size();

    vector<string> nodes;
    vector<int> sums;
    unordered_map<string, int> indices;
    string s(m, '0');
    auto dfs = [&](auto &&self, int i = 0, int sum = 0) {
        if (i == m) {
            nodes.emplace_back(s);
            sums.emplace_back(sum);
            indices[s] = nodes.size() - 1;
            return;
        }

        auto [wi, count] = deduped[i];
        for (int c = 0; c <= min((max_s - sum) / wi, count); c++) {
            s[i] = (char) (c + '0');
            self(self, i + 1, sum + c * wi);
        }
        s[i] = '0';
    };
    dfs(dfs);

    int V = nodes.size();
    vector<vector<int>> adj_list(V);
    for (int v = 0; v < V; v++) {
        int j = -1;
        for (int i = m - 1; ~i; i--)
            if (nodes[v][i] != '0') {
                j = i;
                break;
            }

        if (~j) {
            auto temp = nodes[v];
            temp[j]--;
            adj_list[v].emplace_back(indices[temp]);
        } else j = 0;

        for (; j < m; j++) {
            auto [wj, count] = deduped[j];
            if ((char) (nodes[v][j] - '0') < count)
                if (sums[v] + wj <= max_s) {
                    auto temp = nodes[v];
                    temp[j]++;
                    adj_list[v].emplace_back(indices[temp]);
                }
        }
    }

    vector<vector<int>> vertices(n);
    for (int v = 0; v < V; v++)
        for (int squats = 0; squats < n; squats++)
            if (sums[v] == required[squats]) vertices[squats].emplace_back(v);

    int total = max_s / 10 * 2 * n;
    vector<pair<int, int>> dp1;
    vector<int> dp2(V), dist(V);
    vector<deque<int>> buckets(total);
    auto reset = [&]() {
        fill(dist.begin(), dist.end(), 1e9);
        for (int i = 0; i < total; i++) buckets[i].clear();
        for (auto [v, d] : dp1)
            if (dist[v] > d) {
                dist[v] = d;
                buckets[d].emplace_back(v);
            }
    };

    auto bfs = [&](auto &&check) {
        for (int d = 0; d < total; d++)
            while (!buckets[d].empty()) {
                int v = buckets[d].front();
                buckets[d].pop_front();

                if (dist[v] != d) continue;

                if (check(v, d)) return;

                for (int u : adj_list[v])
                    if (dist[u] > d + 1) {
                        dist[u] = d + 1;
                        if (buckets[d + 1].empty() || dist[u] < dist[buckets[d + 1].front()]) buckets[d + 1].emplace_front(u);
                        else buckets[d + 1].emplace_back(u);
                    }
            }
    };
    
    for (int squats = 0; squats < n; squats++) {
        reset();

        if (!squats) {
            dist[0] = 0;
            buckets[0].emplace_back(0);
        }

        for (int v : vertices[squats]) dp2[v] = -1;

        int remaining = vertices[squats].size();
        bfs([&](int v, int d) {
            if (!~dp2[v]) {
                dp2[v] = d;
                if (!--remaining) return true;
            }
            return false;
        });

        dp1.clear();
        for (int v : vertices[squats]) dp1.emplace_back(v, dp2[v]);
    }

    reset();
    bfs([&](int v, int d) {
        if (!v) {
            cout << d;
            return true;
        }
        return false;
    });
}
