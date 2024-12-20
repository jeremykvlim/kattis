#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Point {
    T x, y;

    Point() {}
    Point(T x, T y) : x(x), y(y) {}

    auto operator<(const Point &p) const {
        return x != p.x ? x < p.x : y < p.y;
    }

    auto operator==(const Point &p) const {
        return x == p.x && y == p.y;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n;
        cin >> n;

        vector<Point<int>> points(n);
        for (auto &[x, y] : points) cin >> x >> y;
        sort(points.begin(), points.end());

        unordered_map<int, set<Point<int>>> X, Y;
        for (int i = 0; i < n; i++) {
            auto [x, y] = points[i];
            X[y].emplace(x, i);
            Y[x].emplace(y, i);
        }

        vector<int> hori(n, -1), verti(n, -1);
        vector<bool> visited(n, false);

        auto dfs = [&](auto &&self, int v = 0) -> void {
            visited[v] = true;
            if (!visited[hori[v]]) self(self, hori[v]);
            if (!visited[verti[v]]) self(self, verti[v]);
        };

        multiset<int> active;
        for (int i = 0; i < n; i++) {
            auto [x, y] = points[i];

            if (!~hori[i]) {
                auto it1 = X[y].upper_bound({x, n});

                if (it1 == X[y].end()) goto next;
                else {
                    hori[i] = it1->y;
                    hori[it1->y] = i;
                    active.emplace(y);
                }
            } else active.erase(active.find(y));

            if (!~verti[i]) {
                auto it1 = Y[x].upper_bound({y, n});

                if (it1 == Y[x].end()) goto next;
                else {
                    auto it2 = active.upper_bound(y);

                    if (it2 != active.end() && *it2 < it1->x) goto next;
                    else {
                        verti[i] = it1->y;
                        verti[it1->y] = i;
                    }
                }
            }
        }
        dfs(dfs);

        if (all_of(visited.begin(), visited.end(), [&](bool b) {return b;})) {
            cout << "YES\n";
            continue;
        }
        next:;
        cout << "NO\n";
    }
}
