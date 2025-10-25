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

    int n, k;
    cin >> n >> k;

    int xl = -1e9, xr = 1e9;
    vector<int> x(n);
    vector<pair<int, int>> points(n + 1, {0, 0});
    for (int i = 0; i < n; i++) {
        cin >> x[i];

        xl = min(xl, x[i]);
        xr = max(xr, x[i]);
        points[i + 1] = {x[i], i + 1};
    }
    sort(points.begin(), points.end());

    vector<int> kth_dist(n + 1, 1e9);
    set<pair<int, int>> s;
    s.emplace(xl - 1, INT_MIN);
    s.emplace(xr + 1, INT_MAX);
    for (int i = n - 1; ~i; i--) {
        auto it_r = s.lower_bound({x[i], INT_MIN}), it_l = prev(it_r);

        int count = 0, kth = INT_MAX;
        for (; count < k; count++) {
            auto l = x[i] - it_l->first, r = it_r->first - x[i];
            if (l <= r) {
                if (it_l->second == INT_MIN) break;
                kth = l;
                it_l = prev(it_l);
            } else {
                if (it_r->second == INT_MAX) break;
                kth = r;
                it_r = next(it_r);
            }
        }
        if (count == k) kth_dist[i + 1] = kth;
        s.emplace(x[i], i);
    }

    auto index = [&](int b, int v) -> int {
        return b * (n + 1) + v;
    };
    int zoom = __lg(xr - xl + 1) + 1;
    vector<int> l(n + 1), r(n + 1), adj_list(zoom * (n + 1) * k, 0);
    for (int b = 0; b < zoom; b++) {
        int radius = 1 << b, left = 0, right = 0;
        for (auto [xi, i] : points)
            if (i) {
                while (left <= n && points[left].first < xi - radius) left++;
                while (right <= n && points[right].first <= xi + radius) right++;
                l[i] = left;
                r[i] = right;
            }

        vector<int> remaining(n + 1, k);
        DisjointSets dsu(n + 2);
        for (int i = n; i; i--)
            if (l[i] < r[i]) {
                int j = dsu.find(l[i]);
                while (j < r[i]) {
                    if (remaining[j]) {
                        adj_list[index(b, points[j].second) * k + k - remaining[j]] = i;
                        if (!--remaining[j]) dsu.sets[j] = dsu.find(j + 1);
                    }
                    j = dsu.find(j + 1);
                }
            }
    }

    vector<int> dist(zoom * (n + 1), -1), ops(n + 1, 1e9);
    dist[index(0, 0)] = 0;
    queue<pair<int, int>> q;
    q.emplace(0, 0);
    while (!q.empty()) {
        auto [b, v] = q.front();
        q.pop();

        int d = dist[index(b, v)];
        if (v) ops[v] = min(ops[v], d + max(b - (kth_dist[v] <= 1 ? -1 : __lg(kth_dist[v] - 1)), 0));

        if (b + 1 < zoom && !~dist[index(b + 1, v)]) {
            dist[index(b + 1, v)] = d + 1;
            q.emplace(b + 1, v);
        }

        if (b && !~dist[index(b - 1, v)]) {
            dist[index(b - 1, v)] = d + 1;
            q.emplace(b - 1, v);
        }

        int t = index(b, v) * k;
        for (int i = 0; i < k; i++) {
            int u = adj_list[t + i];
            if (!u || u == v) continue;
            if (!~dist[index(b, u)]) {
                dist[index(b, u)] = d + 1;
                q.emplace(b, u);
            }
        }
    }

    for (int i = 1; i <= n; i++) cout << (ops[i] == 1e9 ? -1 : ops[i]) << "\n";
}
