#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
    vector<int> sets, value;

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
        value[u_set] += value[v_set];
        return {u_set, v_set};
    }

    int size(int v) {
        return -sets[find(v)];
    }

    DisjointSets(int n) : sets(n, -1), value(n, 1) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, Q;
    cin >> n >> Q;

    auto pairs = [&](int s) {
        return (long long) s * (s - 1) / 2;
    };

    int sets = 1;
    auto count = 0LL;
    DisjointSets dsu(2 * Q);
    set<array<int, 3>> intervals;
    auto merge = [&](int u, int v) {
        auto [big, small] = dsu.unite(u, v);
        if (small != -1) count += pairs(dsu.value[big]) - pairs(dsu.value[big] - dsu.value[small]) - pairs(dsu.value[small]);
    };

    auto add = [&](int l1, int r1) {
        int u = -1, overlap = 0;
        auto it = intervals.lower_bound({l1, r1, u});
        while (it != intervals.end() && (*it)[0] <= r1) {
            auto [l2, r2, v] = *it;
            if (!~u) u = dsu.find(v);
            else {
                merge(u, v);
                u = dsu.find(u);
            }

            overlap += r2 - l2 + 1;
            r1 = max(r1, r2);
            it = intervals.erase(it);
        }

        if (it != intervals.begin() && (*--it)[1] >= l1) {
            auto [l2, r2, v] = *it;
            if (!~u) u = dsu.find(v);
            else {
                merge(u, v);
                u = dsu.find(u);
            }

            overlap += r2 - l2 + 1;
            l1 = min(l1, l2);
            r1 = max(r1, r2);
            intervals.erase(it);
        }

        if (!~u) {
            u = sets++;
            overlap = 1;
        }

        u = dsu.find(u);
        count -= pairs(dsu.value[u]);
        dsu.value[u] += r1 - l1 + 1 - overlap;
        count += pairs(dsu.value[u]);
        intervals.insert({l1, r1, u});
        return u;
    };

    while (Q--) {
        int q, x, y;
        cin >> q >> x >> y;

        if (q == 1) {
            int u = add(x, x), v = add(y, y);
            merge(u, v);
        } else add(x, y);

        cout << count << "\n" << flush;
    }
}