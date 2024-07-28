#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<string> name(n + 1);
    vector<int> c(n + m + 1), d(n + m + 1);
    vector<pair<int, int>> edges(n + m + 1);
    vector<int> indices(n + m + 1);
    for (int i = 1; i <= n; i++) {
        cin >> name[i] >> c[i + m] >> d[i + m];

        edges[i] = {i + m, indices[c[i + m]]};
        indices[c[i + m]] = i;
    }

    for (int i = 1; i <= m; i++) {
        cin >> c[i] >> d[i];

        edges[n + i] = {i, indices[c[i]]};
        indices[c[i]] = n + i;
    }

    vector<int> degree(n + m + 1, 0);
    vector<long long> d1(n + m + 1, 1e10), d2(n + m + 1, 0);
    auto dfs = [&](auto &&self, int v = 0) -> void {
        if (indices[v])
            for (int i = indices[v]; i; degree[v]++) {
                auto [u, j] = edges[i];
                d2[u] = d2[v] + d[u];
                self(self, u);
                d1[v] = min(d1[v], d1[u] + d[v]);
                i = j;
            }
        else d1[v] = d[v];
    };
    dfs(dfs);

    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    vector<pair<long long, int>> sorted(n);
    for (int i = 1; i <= n; i++) {
        pq.emplace(d1[i + m], i + m);
        sorted[i - 1] = {d2[i + m], i + m};
    }
    sort(sorted.begin(), sorted.end());

    vector<int> rank(n + m + 1);
    for (auto [di, i] : sorted) {
        while (!pq.empty()) {
            auto [dj, j] = pq.top();
            if (dj > di) break;
            pq.pop();

            if (!--degree[c[j]] && c[j]) pq.emplace(d1[c[j]], c[j]);
        }
        rank[i] = pq.size() + 1;
    }

    for (int i = 1; i <= n; i++) cout << name[i] << " " << rank[i + m] << "\n";
}
