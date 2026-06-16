#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
    vector<int> sets;

    int find(int v) {
        while (sets[v] >= 0) {
            int p = sets[v];
            if (sets[p] >= 0) sets[v] = sets[p];
            v = p;
        }
        return v;
    }

    pair<int, int> unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set == v_set) return {u_set, -1};

        if (sets[u_set] > sets[v_set]) swap(u_set, v_set);
        sets[u_set] += sets[v_set];
        sets[v_set] = u_set;
        return {u_set, v_set};
    }

    int size(int v) {
        return -sets[find(v)];
    }

    DisjointSets(int n) : sets(n, -1) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, x, t;
    cin >> n >> x >> t;

    int h_max = 0;
    vector<int> h(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> h[i];

        h_max = max(h_max, h[i]);
    }

    vector<int> d(n + 1);
    for (int i = 1; i <= n; i++) cin >> d[i];

    vector<vector<int>> indices(h_max + 2);
    for (int i = 1; i <= n; i++) indices[h[i]].emplace_back(i);

    DisjointSets dsu(h_max + 2);
    vector<int> rep(h_max + 2);
    iota(rep.begin(), rep.end(), 0);

    vector<long long> dist(n + 1, -1);
    dist[x] = 0;
    queue<int> q;
    q.emplace(x);
    while (!q.empty()) {
        int j = q.front();
        q.pop();

        int l = rep[dsu.find(h[j])], r = min(h_max, h[j] + d[j]);
        for (int curr = l; curr <= r; curr = rep[dsu.find(curr)]) {
            for (int i : indices[curr])
                if (dist[i] == -1) {
                    dist[i] = dist[j] + 1;
                    q.emplace(i);
                }

            auto [big, small] = dsu.unite(curr + 1, curr);
            if (small != -1) rep[big] = max(rep[big], rep[small]);
        }
    }
    cout << (!~dist[1] ? dist[1] : dist[1] * t);
}