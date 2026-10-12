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

    int n, K, D, Q;
    cin >> n >> K >> D >> Q;

    vector<int> d(n);
    for (int &di : d) cin >> di;

    vector<vector<int>> indices(K);
    for (int i = 0; i < n; i++) {
        int ki;
        cin >> ki;

        indices[ki].emplace_back(i);
    }

    vector<int> offset(2 * K + 1, 0), id(n);
    for (int ki = 0; ki < K; ki++) {
        sort(indices[ki].begin(), indices[ki].end(), [&](int i, int j) { return d[i] < d[j]; });

        offset[ki + 1] = offset[ki] + indices[ki].size();
        for (int j = 0; j < indices[ki].size(); j++) id[indices[ki][j]] = offset[ki] + j;
    }
    for (int ki = 0; ki < K; ki++) offset[K + ki + 1] = offset[ki + 1] + n;

    auto search = [&](int ki, int di) -> pair<int, long long> {
        auto it = lower_bound(indices[ki].begin(), indices[ki].end(), di, [&](int i, int x) { return d[i] < x; });
        int i = it == indices[ki].end() ? indices[ki][0] : *it;
        long long diff = d[i] - di;
        if (diff < 0) diff += D;
        return {i, diff};
    };

    int m = 2 * n;
    vector<int> next(m, m);
    vector<long long> w(m + 1, 0);
    for (int ki = 0; ki < K; ki++)
        for (int i : indices[ki]) {
            auto [j, diff] = search((ki + 1) % K, d[i]);
            int u = id[i], v = id[j];
            next[u] = v + (ki == K - 1 ? n : 0);
            w[u] = diff;
            if (ki < K - 1) {
                next[u + n] = v + n;
                w[u + n] = diff;
            }
        }
    for (int v = m - 1; ~v; v--) w[v] += w[next[v]];

    vector<tuple<int, int, int, long long>> order;
    for (int q = 0; q < Q; q++) {
        int dq, kq, nq;
        cin >> dq >> kq >> nq;

        if (!nq) continue;
        auto [i, diff] = search(kq, dq);
        order.emplace_back(offset[kq + nq] - 1, id[i], q, diff);
    }
    sort(order.begin(), order.end());

    vector<pair<int, int>> queries;
    for (auto [bound, i, q, diff] : order) queries.emplace_back(i, bound);

    vector<long long> dist(Q);
    auto result = monotone_jumps(next, queries);
    for (int i = 0; i < order.size(); i++) {
        auto [bound, j, q, diff] = order[i];
        dist[q] = diff + w[j] - w[result[i].second];
    }
    for (auto di : dist) cout << di << "\n";
}
