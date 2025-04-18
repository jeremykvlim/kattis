#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
    vector<int> sets, members;

    int find(int v) {
        return sets[v] == v ? v : (sets[v] = find(sets[v]));
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set != v_set) {
            sets[v_set] = u_set;
            members[u_set] |= members[v_set];
            return true;
        }
        return false;
    }

    void reroot() {
        for (int i = 0; i < sets.size(); i++) {
            sets[i] = sets[find(i)];
            members[i] = members[find(i)];
        }
    }

    DisjointSets(int n) : sets(n), members(n) {
        iota(sets.begin(), sets.end(), 0);
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
    dsu.reroot();

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
                        d = min(d, dist[u][v] + self(self, count ^ ((1 << u) | (1 << v)), curr | dsu.members[v]));

        return memo[curr | (count << n)] = d;
    };

    cout << base + dp(dp, odd_count, dsu.members[0]);
}
