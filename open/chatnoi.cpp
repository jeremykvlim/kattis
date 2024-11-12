#include <bits/stdc++.h>
using namespace std;

struct Hash {
    size_t operator()(pair<int, int> p) const {
        auto h = hash<int>()(p.first);
        h ^= hash<int>()(p.second) + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<string> w(n);
    for (auto &wi : w) cin >> wi;
    auto sorted = w;
    sort(sorted.begin(), sorted.end());
    sorted.erase(unique(sorted.begin(), sorted.end()), sorted.end());

    vector<int> indices(n);
    for (int i = 0; i < n; i++) indices[i] = lower_bound(sorted.begin(), sorted.end(), w[i]) - sorted.begin();

    vector<vector<int>> kgrams(n - k + 1, vector<int>(k));
    for (int l = 0, r = k - 1; r < n; l++, r++)
        for (int i = 0; i <= r - l; i++) kgrams[l][i] = indices[i + l];
    sort(kgrams.begin(), kgrams.end());
    kgrams.erase(unique(kgrams.begin(), kgrams.end()), kgrams.end());

    vector<int> pos(n, -1);
    for (int l = 0, r = k - 1; r < n; l++, r++) {
        vector<int> temp(k);
        for (int i = 0; i <= r - l; i++) temp[i] = indices[i + l];

        auto it = lower_bound(kgrams.begin(), kgrams.end(), temp);
        if (it == kgrams.end() || *it != temp) continue;
        pos[r] = it - kgrams.begin();
    }

    unordered_map<pair<int, int>, int, Hash> edges;
    for (int r = k - 1; r < n - 1; r++) edges[{pos[r], pos[r + 1]}]++;

    int V = kgrams.size();
    vector<int> freqs;
    vector<vector<pair<int, int>>> adj_list_regular(V), adj_list_transpose(V);
    for (auto [e, f] : edges) {
        auto [u, v] = e;
        adj_list_regular[u].emplace_back(v, f);
        adj_list_transpose[v].emplace_back(u, f);
        freqs.emplace_back(f);
    }
    sort(freqs.rbegin(), freqs.rend());
    freqs.erase(unique(freqs.begin(), freqs.end()), freqs.end());

    vector<vector<int>> longest(freqs.size(), vector<int>(V, -1)), next(freqs.size(), vector<int>(V, -1));
    for (int i = 0; i < freqs.size(); i++) {
        vector<int> degree(V, 0);
        for (int v = 0; v < V; v++)
            for (auto [u, f] : adj_list_regular[v])
                if (f >= freqs[i]) degree[v]++;

        queue<int> q;
        for (int v = 0; v < V; v++)
            if (!degree[v]) {
                longest[i][v] = 0;
                q.emplace(v);
            }

        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (auto [u, f] : adj_list_transpose[v])
                if (f >= freqs[i])
                    if (!--degree[u]) q.emplace(u);

            for (auto [u, f] : adj_list_regular[v])
                if (f >= freqs[i]) longest[i][v] = max(longest[i][v], longest[i][u] + 1);
        }

        for (int v = 0; v < V; v++)
            if (!~longest[i][v]) longest[i][v] = 1e6;

        for (int v = 0; v < V; v++)
            for (auto [u, f] : adj_list_regular[v])
                if (f >= freqs[i]) {
                    if (!~next[i][v]) next[i][v] = u;
                    else if (longest[i][next[i][v]] < longest[i][u]) next[i][v] = u;
                }
    }
    indices.resize(k);

    int q;
    cin >> q;

    while (q--) {
        int m;
        cin >> m;

        for (int &i : indices) {
            string ui;
            cin >> ui;

            i = lower_bound(sorted.begin(), sorted.end(), ui) - sorted.begin();
        }

        vector<string> words(m, sorted[0]);
        int v = lower_bound(kgrams.begin(), kgrams.end(), indices) - kgrams.begin();
        if (~v)
            for (int i = 0; i < freqs.size(); i++)
                if (longest[i][v] >= m) {
                    for (auto &wj : words) {
                        v = next[i][v];
                        wj = sorted[kgrams[v].back()];
                    }
                    break;
                }

        for (auto wj : words) cout << wj << " ";
        cout << "\n";
    }
}
