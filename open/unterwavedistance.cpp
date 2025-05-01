#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> g(n);
    vector<char> d(n);
    vector<pair<int, int>> humans, aliens;
    for (int i = 0; i < n; i++) {
        cin >> g[i] >> d[i];

        (d[i] == 'h' ? humans : aliens).emplace_back(g[i], i);
    }
    sort(humans.begin(), humans.end());
    sort(aliens.begin(), aliens.end());

    int H = humans.size(), A = aliens.size();
    vector<int> g_h(H), g_a(A), indices_h(n, -1), indices_a(n, -1);
    vector<long long> cb_g_h(H), cb_g_a(A);

    auto cb = [&](long long x) { return x * x * x; };
    for (int i = 0; i < H; i++) {
        g_h[i] = humans[i].first;
        indices_h[humans[i].second] = i;
        cb_g_h[i] = cb(g_h[i]);
    }

    for (int i = 0; i < A; i++) {
        g_a[i] = aliens[i].first;
        indices_a[aliens[i].second] = i;
        cb_g_a[i] = cb(g_a[i]);
    }

    auto dist = LLONG_MAX;
    auto update = [&](auto c) {
        dist = min(dist, abs(c));
    };
    for (int h = 0, a = 0; h < H && a < A;) {
        update(cb_g_h[h] - cb_g_a[a]);

        if (g_h[h] < g_a[a]) h++;
        else a++;
    }

    int e;
    cin >> e;

    vector<vector<int>> adj_list(n);
    while (e--) {
        int u, v;
        cin >> u >> v;

        adj_list[u - 1].emplace_back(v - 1);
        adj_list[v - 1].emplace_back(u - 1);
    }

    for (int v = 0; v < n; v++) {
        adj_list[v].emplace_back(v);
        vector<int> neighbor_h, neighbor_a;
        for (int u : adj_list[v]) (d[u] == 'h' ? neighbor_h : neighbor_a).emplace_back(u);

        vector<int> i_a;
        if (A > neighbor_a.size()) {
            for (int a : neighbor_a) i_a.emplace_back(indices_a[a]);
            sort(i_a.begin(), i_a.end());

            for (int h : neighbor_h) {
                int g_h1 = g[h] + (h == v ? -1 : 1), i = lower_bound(g_a.begin(), g_a.end(), g_h1) - g_a.begin();

                int l = i - 1;
                for (; ~l && binary_search(i_a.begin(), i_a.end(), l); l--);
                if (~l) update(cb_g_a[l] - cb(g_h1));

                int r = i;
                for (; r < A && binary_search(i_a.begin(), i_a.end(), r); r++);
                if (r < A) update(cb_g_a[r] - cb(g_h1));
            }
        }

        vector<int> i_h;
        if (H > neighbor_h.size()) {
            for (int h : neighbor_h) i_h.emplace_back(indices_h[h]);
            sort(i_h.begin(), i_h.end());

            for (int a : neighbor_a) {
                int g_a1 = g[a] + (a == v ? -1 : 1), i = lower_bound(g_h.begin(), g_h.end(), g_a1) - g_h.begin();

                int l = i - 1;
                for (; ~l && binary_search(i_h.begin(), i_h.end(), l); l--);
                if (~l) update(cb_g_h[l] - cb(g_a1));

                int r = i;
                for (; r < H && binary_search(i_h.begin(), i_h.end(), r); r++);
                if (r < H) update(cb_g_h[r] - cb(g_a1));
            }
        }

        for (int h : neighbor_h) {
            int g_h1 = g[h] + (h == v ? -1 : 1);
            for (int a : neighbor_a) {
                int g_a1 = g[a] + (a == v ? -1 : 1);
                update(cb(g_a1) - cb(g_h1));
            }
        }
    }
    cout << dist;
}
