#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
    vector<int> sets;

    int find(int v) {
        return sets[v] == v ? v : (sets[v] = find(sets[v]));
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set != v_set) {
            sets[v_set] = u_set;
            return true;
        }
        return false;
    }

    DisjointSets(int n) : sets(n) {
        iota(sets.begin(), sets.end(), 0);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> p(n), indices(n + 1);
    for (int i = 0; i < n; i++) {
        cin >> p[i];

        indices[p[i]] = i;
    }

    vector<bool> swapper(n, false), swappee(n, false);
    for (int pi = n; pi; pi--) {
        int i = indices[pi];
        if (!i || i == n - 1 || swapper[i - 1] || swapper[i + 1]) swappee[i] = true;
        else swapper[i] = true;
    }

    auto q = p;
    for (int il = 0, ir = 0; il < n; il = ++ir) {
        for (; ir + 1 < n && swappee[ir] != swappee[ir + 1]; ir++);
        if (il < ir) {
            vector<int> a, b;
            for (int i = il; i <= ir; i++) (!((i - il) & 1) ? a : b).emplace_back(p[i]);

            int m = a.size();
            vector<array<int, 3>> sweep;
            for (int j = 0; j < m; j++) {
                sweep.push_back({a[j], 0, j});
                if (j < m - 1) sweep.push_back({b[j], 1, j});
            }
            sort(sweep.begin(), sweep.end());

            set<int> swappers;
            vector<int> l(m), r(m);
            for (auto [_, type, j] : sweep)
                if (type) swappers.emplace(j);
                else {
                    auto it = swappers.lower_bound(j);
                    l[j] = it == swappers.begin() ? 0 : *prev(it) + 1;
                    r[j] = it == swappers.end() ? m - 1 : *it;
                }

            vector<pair<int, int>> order;
            for (int j = 0; j < m; j++) order.emplace_back(a[j], j);
            sort(order.rbegin(), order.rend());

            vector<int> temp(m);
            DisjointSets dsu(m + 1);
            set<pair<int, int>> s;
            for (auto [aj, j] : order) {
                auto it1 = s.upper_bound({j, m});

                int lj = max(l[j], it1 == s.begin() ? 0 : prev(it1)->second + 1), rj = min(r[j], it1 == s.end() ? m - 1 : it1->first - 1), k = dsu.find(rj + 1);
                temp[k - 1] = aj;
                dsu.unite(k - 1, k);
                if (lj + 1 > dsu.find(rj + 1)) {
                    auto it2 = s.lower_bound({lj, -1});
                    if (it2 != s.begin()) {
                        auto it3 = prev(it2);
                        if (lj < it3->second + 2) it2 = it3;
                    }

                    for (int bound = rj + 2; it2 != s.end() && it2->first < bound; it2 = s.erase(it2)) {
                        lj = min(lj, it2->first);
                        rj = max(rj, it2->second);
                    }
                    s.emplace(lj, rj);
                }
            }
            for (int j = 0; j < m; j++) q[il + 2 * j] = temp[j];
        }
    }

    for (int qi : q) cout << qi << " ";
}