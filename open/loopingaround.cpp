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

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set == v_set) return false;

        if (sets[u_set] > sets[v_set]) swap(u_set, v_set);
        sets[u_set] += sets[v_set];
        sets[v_set] = u_set;
        return true;
    }

    int size(int v) {
        return -sets[find(v)];
    }

    DisjointSets(int n) : sets(n, -1) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n;
        cin >> n;

        vector<pair<int, int>> points(n);
        for (auto &[x, y] : points) cin >> x >> y;

        bool possible = true;
        DisjointSets dsu(n);
        vector<int> hori(n), order(n);
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](int i, int j) { return points[i].second != points[j].second ? points[i].second < points[j].second : points[i].first < points[j].first; });
        for (int i = 0, j = 1; i < n; i = j++) {
            for (; j < n && points[order[i]].second == points[order[j]].second; j++);
            if ((j - i) & 1) {
                possible = false;
                break;
            }

            for (int k = i; k < j; k += 2) {
                int u = order[k], v = order[k + 1];
                hori[u] = v;
                hori[v] = u;
                dsu.unite(u, v);
            }
        }

        if (!possible) {
            cout << "NO\n";
            continue;
        }

        sort(order.begin(), order.end(), [&](int i, int j) { return points[i].first != points[j].first ? points[i].first < points[j].first : points[i].second < points[j].second; });
        set<int> active;
        for (int i = 0, j = 1; i < n; i = j++) {
            for (; j < n && points[order[i]].first == points[order[j]].first; j++);
            if ((j - i) & 1) {
                possible = false;
                break;
            }

            for (int k = i; k < j; k++) {
                int v = order[k];
                if (points[order[i]].first < points[hori[v]].first) active.emplace(points[v].second);
            }

            for (int k = i; k < j; k += 2) {
                int u = order[k], v = order[k + 1];
                dsu.unite(u, v);
                auto it = active.upper_bound(points[u].second);
                if (it != active.end() && *it < points[v].second) {
                    possible = false;
                    break;
                }
            }
            if (!possible) break;

            for (int k = i; k < j; k++) {
                int v = order[k];
                if (points[order[i]].first > points[hori[v]].first) active.erase(points[v].second);
            }
        }
        cout << (possible && dsu.size(0) == n ? "YES\n" : "NO\n");
    }
}
