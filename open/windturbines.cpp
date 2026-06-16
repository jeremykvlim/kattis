#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct FenwickTree {
    vector<T> BIT;

    void update(int i, T v) {
        for (; i && i < BIT.size(); i += i & -i) BIT[i] += v;
    }

    T pref_sum(int i) {
        T sum = 0;
        for (; i; i &= i - 1) sum += BIT[i];
        return sum;
    }

    T range_sum_query(int l, int r) {
        if (l >= r) return 0;
        return pref_sum(r) - pref_sum(l);
    }

    FenwickTree(int n) : BIT(n, 0) {}
};

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

    int n, m, Q;
    cin >> n >> m >> Q;

    vector<array<int, 3>> edges(m);
    for (auto &[u, v, c] : edges) cin >> u >> v >> c;

    vector<int> indices(m);
    iota(indices.begin(), indices.end(), 0);
    sort(indices.begin(), indices.end(), [&](int i, int j) { return edges[i][2] < edges[j][2]; });

    DisjointSets dsu(n);
    vector<set<int>> members(n);
    for (int i = 0; i < n; i++) members[i].emplace(i);

    auto base = 0LL;
    vector<vector<pair<int, int>>> sweep(n);
    for (int i : indices) {
        auto [u, v, w] = edges[i];
        auto [big, small] = dsu.unite(u, v);
        if (small == -1) continue;

        base += w;
        for (int r : members[small]) {
            auto it = members[big].lower_bound(r);
            if (it != members[big].begin()) sweep[r].emplace_back(*prev(it), i);
            if (it != members[big].end()) sweep[*it].emplace_back(r, i);
        }
        members[big].insert(members[small].begin(), members[small].end());
        members[small].clear();
    }


    vector<vector<pair<int, int>>> queries(n);
    for (int i = 0; i < Q; i++) {
        int l, r;
        cin >> l >> r;

        queries[r].emplace_back(l, i);
    }

    vector<int> prev(m, -1);
    FenwickTree<long long> fw(n + 1);
    vector<long long> cost(Q, 0LL);
    for (int r = 0; r < n; r++) {
        for (auto [l, i] : sweep[r]) {
            if (prev[i] >= l) continue;
            if (~prev[i]) fw.update(prev[i] + 1, -edges[i][2]);
            prev[i] = l;
            fw.update(l + 1, edges[i][2]);
        }

        for (auto [l, q] : queries[r]) cost[q] = base - (fw.pref_sum(n) - fw.pref_sum(l));
    }

    for (auto c : cost) cout << c << "\n";
}
