#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
    vector<int> sets, members;

    int find(int v) {
        while (sets[v] >= 0) {
            int p = sets[v];
            if (sets[p] >= 0) sets[v] = sets[p];
            v = p;
        }
        return v;
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set == v_set) return false;

        if (sets[u_set] > sets[v_set]) swap(u_set, v_set);
        sets[u_set] += sets[v_set];
        sets[v_set] = u_set;
        members[u_set] |= members[v_set];
        return true;
    }

    int size(int v) {
        return -sets[find(v)];
    }

    DisjointSets(int n) : sets(n, -1), members(n) {
        for (int i = 0; i < n; i++) members[i] = 1 << i;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, r;
    cin >> n >> r;

    vector<vector<int>> dist(n, vector<int>(n, 1e9));
    int base = 0, odd_count = 0, all = 0;
    DisjointSets dsu(n);
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
            if ((curr >> u) & 1 && (count >> u) & 1)
                for (int v = 0; v < n; v++)
                    if (u != v && dist[u][v] < 1e9 && (curr >> v) & 1 && (count >> v) & 1)
                        d = min(d, dist[u][v] + self(self, count ^ ((1 << u) | (1 << v)), curr));

        for (int u = 0; u < n; u++)
            if ((curr >> u) & 1)
                for (int v = 0; v < n; v++)
                    if (u != v && dist[u][v] < 1e9 && !((curr >> v) & 1) && (all >> v) & 1)
                        d = min(d, dist[u][v] + self(self, count ^ ((1 << u) | (1 << v)), curr | dsu.members[dsu.find(v)]));

        return memo[curr | (count << n)] = d;
    };

    cout << base + dp(dp, odd_count, dsu.members[dsu.find(0)]);
}