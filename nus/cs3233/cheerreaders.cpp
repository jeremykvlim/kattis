#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
    int n;
    unordered_map<int, int> sets, size, prev;

    int add_set() {
        sets[n] = n;
        size[n] = 1;
        prev[n] = -1;
        return n++;
    }

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

    DisjointSets() : n(0) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, Q;
    cin >> n >> Q;

    auto pairs = [&](int s) {
        return (long long) s * (s - 1) / 2;
    };

    auto count = 0LL;
    DisjointSets dsu;
    set<array<int, 3>> intervals;
    auto merge = [&](int v, int u) {
        if (dsu.unite(v, u)) {
            int v_set = dsu.find(v), u_set = dsu.prev[u];
            count += pairs(dsu.size[v_set]) - pairs(dsu.size[v_set] - dsu.size[u_set]) - pairs(dsu.size[u_set]);
        }
    };

    auto add = [&](int l1, int r1) {
        int v = -1, overlap = 0;
        auto it = intervals.lower_bound({l1, r1, v});
        while (it != intervals.end() && (*it)[0] <= r1) {
            auto [l2, r2, u] = *it;
            if (!~v) v = dsu.find(u);
            else {
                merge(v, u);
                v = dsu.find(v);
            }

            overlap += r2 - l2 + 1;
            r1 = max(r1, r2);
            it = intervals.erase(it);
        }

        if (it != intervals.begin() && (*--it)[1] >= l1) {
            auto [l2, r2, u] = *it;
            if (!~v) v = dsu.find(u);
            else {
                merge(v, u);
                v = dsu.find(v);
            }

            overlap += r2 - l2 + 1;
            l1 = min(l1, l2);
            r1 = max(r1, r2);
            intervals.erase(it);
        }

        if (!~v) {
            v = dsu.add_set();
            overlap = 1;
        }

        count -= pairs(dsu.size[v]);
        dsu.size[v] += r1 - l1 + 1 - overlap;
        count += pairs(dsu.size[v]);
        intervals.insert({l1, r1, v});
        return v;
    };

    while (Q--) {
        int q, x, y;
        cin >> q >> x >> y;

        if (q == 1) {
            int v = add(x, x), u = add(y, y);
            merge(v, u);
        } else add(x, y);

        cout << count << "\n" << flush;
    }
}
