#include <bits/stdc++.h>
using namespace std;

struct DisjointSet {
    vector<int> sets, components;

    int find(int p) {
        return (sets[p] == p) ? p : (sets[p] = find(sets[p]));
    }

    bool unite(int p, int q) {
        int p_set = find(p), q_set = find(q);
        if (p_set != q_set) {
            sets[q_set] = p_set;
            components[p_set] |= components[q_set];
            return true;
        }
        return false;
    }

    DisjointSet(int n) : sets(n), components(n) {
        iota(sets.begin(), sets.end(), 0);
        for (int i = 0; i < n; i++) components[i] = 1 << i;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, r;
    cin >> n >> r;

    vector<vector<int>> dist(n, vector<int>(n, 1e9));
    int base = 0, odd_count = 0, all = 0;
    DisjointSet dsu(n);
    while (r--) {
        int a, b, c;
        cin >> a >> b >> c;
        a--;
        b--;

        dist[a][b] = dist[b][a] = c;
        base += c;
        odd_count ^= (1 << a) | (1 << b);
        all |= (1 << a) | (1 << b);
        dsu.unite(a, b);
    }
    for (int i = 0; i < n; i++) dsu.components[i] = dsu.components[dsu.find(i)];

    int f;
    cin >> f;

    while (f--) {
        int a, b, c;
        cin >> a >> b >> c;
        a--;
        b--;

        dist[a][b] = dist[b][a] = min(dist[a][b], c);
    }

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++) dist[j][k] = min(dist[j][k], dist[j][i] + dist[i][k]);

    unordered_map<int, int> memo;
    auto dp = [&](auto &&self, int count, int curr) {
        if (!count && (curr & all) == all) return 0;
        if (memo.count(curr | (count << n))) return memo[curr | (count << n)];

        int d = 1e9;
        for (int u = 0; u < n; u++)
            if (curr & (1 << u) && count & (1 << u))
                for (int v = 0; v < n; v++)
                    if (u != v && dist[u][v] < 1e9 && (curr & (1 << v)) && (count & (1 << v)))
                        d = min(d, dist[u][v] + self(self, count ^ ((1 << u) | (1 << v)), curr));

        for (int u = 0; u < n; u++)
            if (curr & (1 << u))
                for (int v = 0; v < n; v++)
                    if (u != v && dist[u][v] < 1e9 && !(curr & (1 << v)) && (all & (1 << v)))
                        d = min(d, dist[u][v] + self(self, count ^ ((1 << u) | (1 << v)), curr | dsu.components[v]));

        return memo[curr | (count << n)] = d;
    };

    cout << base + dp(dp, odd_count, dsu.components[0]);
}
