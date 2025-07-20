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

    FenwickTree(int n) : BIT(n, 0) {}
};

struct DisjointSets {
    vector<int> sets, size;

    int find(int v) {
        return sets[v] == v ? v : (sets[v] = find(sets[v]));
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set != v_set) {
            if (size[u_set] < size[v_set]) swap(u_set, v_set);
            sets[v_set] = u_set;
            size[u_set] += size[v_set];
            return true;
        }
        return false;
    }

    DisjointSets(int n) : sets(n), size(n, 1) {
        iota(sets.begin(), sets.end(), 0);
    }
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
        int u_set = dsu.find(u), v_set = dsu.find(v);
        if (!dsu.unite(u, v)) continue;

        base += w;
        if (dsu.size[u_set] < dsu.size[v_set]) swap(u_set, v_set);
        for (int r : members[v_set]) {
            auto it = members[u_set].lower_bound(r);
            if (it != members[u_set].begin()) sweep[r].emplace_back(*prev(it), i);
            if (it != members[u_set].end()) sweep[*it].emplace_back(r, i);
        }
        members[u_set].insert(members[v_set].begin(), members[v_set].end());
        members[v_set].clear();
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
