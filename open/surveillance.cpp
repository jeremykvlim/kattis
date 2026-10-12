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

vector<pair<int, int>> monotone_jumps(const vector<int> &next, const vector<pair<int, int>> &queries) {
    int n = next.size();
    vector<vector<int>> adj_list(n + 1);
    vector<int> depth(n + 1, 0);
    for (int v = 0; v < n; v++) adj_list[next[v]].emplace_back(v);
    for (int v = n - 1; ~v; v--) depth[v] = depth[next[v]] + 1;

    DisjointSets dsu(n + 1);
    vector<int> rep(n + 1);
    iota(rep.begin(), rep.end(), 0);

    int q = queries.size();
    vector<pair<int, int>> result(q);
    for (int i = 0, v = 0; i < q; i++) {
        auto [a, bound] = queries[i];
        for (; v <= bound; v++)
            for (int u : adj_list[v]) {
                auto [big, small] = dsu.unite(u, v);
                if (small != -1) rep[big] = max(rep[big], rep[small]);
            }

        int b = rep[dsu.find(a)];
        result[i] = {depth[a] - depth[b], b};
    }
    return result;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    int m = 2 * n, reach = -1;
    vector<int> furthest(m, -1);
    for (int i = 0; i < k; i++) {
        int a, b;
        cin >> a >> b;
        a--;
        b--;

        int r = b;
        if (a > b) {
            r += n;
            reach = max(reach, b);
        }
        furthest[a] = max(furthest[a], r);
        furthest[a + n] = max(furthest[a + n], r + n);
    }

    vector<int> next(m, m);
    for (int i = 0; i < m; i++) {
        reach = max(reach, furthest[i]);
        if (reach < i) {
            cout << "impossible";
            exit(0);
        }
        next[i] = min(next[i], reach + 1);
    }

    vector<pair<int, int>> queries(n);
    for (int i = 0; i < n; i++) queries[i] = {i, i + n - 1};

    int cameras = n;
    for (auto [cost, v] : monotone_jumps(next, queries)) cameras = min(cameras, cost + 1);
    cout << cameras;
}