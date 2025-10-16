#include <bits/stdc++.h>
using namespace std;

tuple<vector<int>, vector<int>, int> hopcroft_karp(int n, int m, const vector<pair<int, int>> &edges) {
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

    int n, k;
    cin >> n >> k;

    vector<int> a(k);
    for (int &ai : a) cin >> ai;

    vector<int> happiness;
    vector<vector<int>> h(n, vector<int>(k));
    for (auto &row : h)
        for (int &hij : row) {
            cin >> hij;

            happiness.emplace_back(hij);
        }
    sort(happiness.begin(), happiness.end());
    happiness.erase(unique(happiness.begin(), happiness.end()), happiness.end());

    vector<int> pref(k + 1, 0);
    for (int i = 0; i < k; i++) pref[i + 1] = pref[i] + a[i];

    int diff = INT_MAX;
    for (int l = 0, r = 0; l < happiness.size(); l++) {
        auto valid = [&](int hl, int hr) {
            vector<pair<int, int>> edges;
            for (int i = 0; i < n; i++)
                for (int j = 0; j < k; j++)
                    if (hl <= h[i][j] && h[i][j] <= hr)
                        for (int p = pref[j]; p < pref[j + 1]; p++) edges.emplace_back(i, p);
            auto [_, __, matches] = hopcroft_karp(n, n, edges);
            return matches == n;
        };

        while (r < happiness.size() && !valid(happiness[l], happiness[r])) r++;
        if (r == happiness.size()) break;
        diff = min(diff, happiness[r] - happiness[l]);
    }
    cout << diff;
}
