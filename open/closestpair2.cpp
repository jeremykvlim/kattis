#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n && n) {
        vector<pair<double, double>> points(n);
        for (auto &[x, y] : points) cin >> x >> y;
        sort(points.begin(), points.end());

        auto dist = DBL_MAX;
        pair<double, double> p1, p2;
        set<pair<double, double>> s;
        for (int i = 0, j = 0; i < n; i++) {
            while (j < i && points[i].first - points[j].first >= dist) {
                s.erase({points[j].second, points[j].first});
                j++;
            }

            for (auto it = s.lower_bound({points[i].second - dist, -1e6}); it != s.upper_bound({points[i].second + dist, 1e6}); it++) {
                auto d = hypot(points[i].first - it->second, points[i].second - it->first);

                if (dist > d) {
                    dist = d;
                    p1 = points[i];
                    p2 = {it->second, it->first};
                }
            }

            s.emplace(points[i].second, points[i].first);
        }

        cout << fixed << setprecision(2) << p1.first << " " << p1.second << " " << p2.first << " " << p2.second << "\n";
    }
}
