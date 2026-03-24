#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
    vector<int> sets;

    int find(int v) {
        return sets[v] == v ? v : (sets[v] = find(sets[v]));
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set != v_set) {
            sets[v_set] = u_set;
            return true;
        }
        return false;
    }

    DisjointSets(int n) : sets(n) {
        iota(sets.begin(), sets.end(), 0);
    }
};

pair<int, bool> kruskal(int n, vector<array<int, 3>> edges) {
    DisjointSets dsu(n);
    sort(edges.begin(), edges.end());

    int count = 0, mst_max = 0;
    for (auto e : edges) {
        auto [w, u, v] = e;
        if (dsu.unite(u, v)) {
            mst_max = max(mst_max, w);
            if (++count == n - 1) break;
        }
    }

    return {mst_max, count == n - 1};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, w, d;
    cin >> n >> w >> d;

    vector<int> f(n);
    for (int &fi : f) cin >> fi;
    for (int i = 0; i < n; i++) f.emplace_back((f[i] + w) % d);

    vector<int> order(2 * n), rev(2 * n), sorted(2 * n);
    iota(order.rbegin(), order.rend(), 0);
    stable_sort(order.begin(), order.end(), [&](int i, int j) { return f[i] < f[j]; });
    for (int i = 0; i < 2 * n; i++) {
        rev[order[i]] = i;
        sorted[i] = f[order[i]];
    }
    sorted.insert(sorted.begin(), 0);
    sorted.emplace_back(d);

    auto base = (long long) n * (w / d) * d - w;
    w %= d;

    vector<int> overlaps{0};
    for (int i = 0; i < n; i++)
        if (f[i] + w >= d) overlaps[0]++;

    int o = overlaps[0];
    for (int i : order) {
        overlaps.emplace_back(overlaps.back() + (i < n ? 1 : -1));
        o = max(o, overlaps.back());
    }

    auto time = LLONG_MAX;
    for (int _ = 0, m = 2 * n + 1; _ < 2; _++, o++) {
        int r = 0;
        bool possible = true;
        vector<array<int, 3>> edges;
        for (int i = 0; i < m; i++) {
            if (overlaps[i] > o) {
                possible = false;
                break;
            }

            if (overlaps[i] == o) {
                if (i == 2 * n) {
                    possible = false;
                    break;
                }
                r = max(r, sorted[i + 1]);
            } else if (overlaps[i] == o - 1 && i < 2 * n) edges.push_back({sorted[i + 1], i, (i + 1) % m});
            else if (overlaps[i] < o - 1) edges.push_back({0, i, (i + 1) % m});
        }
        if (!possible) continue;

        for (int i = 0; i < n; i++) edges.push_back({0, rev[i] + 1, rev[i + n] + 1});
        auto [mst_max, valid_mst] = kruskal(m, edges);
        if (valid_mst) time = min(time, base + (long long) (o - 1) * d + max(r, mst_max));
    }
    cout << time;
}
