#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
    vector<int> sets, size, prev;

    int find(int v) {
        return sets[v] == v ? v : (sets[v] = find(sets[v]));
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set != v_set) {
            sets[v_set] = u_set;
            size[u_set] += size[v_set];
            prev[v] = v_set;
            return true;
        }
        return false;
    }

    DisjointSets(int n) : sets(n), size(n, 1), prev(n, -1) {
        iota(sets.begin(), sets.end(), 0);
    }
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
        if (dsu.unite(u, v)) {
            int u_set = dsu.find(u), v_set = dsu.prev[v];
            count += pairs(dsu.size[u_set]) - pairs(dsu.size[u_set] - dsu.size[v_set]) - pairs(dsu.size[v_set]);
        }
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

        count -= pairs(dsu.size[u]);
        dsu.size[u] += r1 - l1 + 1 - overlap;
        count += pairs(dsu.size[u]);
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
