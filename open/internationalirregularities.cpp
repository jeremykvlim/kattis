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

    int n, Q, m;
    cin >> n >> Q >> m;

    vector<int> r(n), t(n);
    for (int &ri : r) cin >> ri;
    for (int &ti : t) cin >> ti;

    vector<int> next(n), days(n);
    for (int i = 0, j = 0; i < n; i++) {
        for (; j + 1 < n && r[j + 1] - r[i] <= m; j++);
        next[i] = j;
        days[i] = i ? min(days[i - 1], t[i]) : t[i];
    }

    for (int i = n - 1; ~i; i--)
        if (next[i] != i) days[i] = min(days[i], days[next[i]] + 1);

    vector<int> time(Q);
    vector<array<int, 3>> order;
    for (int q = 0; q < Q; q++) {
        int x, y;
        cin >> x >> y;
        x--;
        y--;

        if (x <= y) {
            time[q] = 1;
            continue;
        }

        time[q] = min(t[y] + 1, days[next[y]] + 2);
        order.push_back({x, y, q});
    }
    sort(order.begin(), order.end());

    vector<pair<int, int>> queries;
    for (auto [x, y, id] : order) queries.emplace_back(y, x - 1);

    auto result = monotone_jumps(next, queries);
    for (int i = 0; i < result.size(); i++) {
        auto [cost, v] = result[i];
        if (next[v] >= order[i][0]) time[order[i][2]] = min(time[order[i][2]], cost + 1);
    }
    for (int ti : time) cout << ti << "\n";
}