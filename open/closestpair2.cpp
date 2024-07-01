#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n && n) {
        vector<pair<double, double>> points(n);
        vector<pair<pair<double, double>, int>> sorted(n);
        for (int i = 0; i < n; i++) {
            cin >> points[i].first >> points[i].second;

            sorted[i] = {points[i], i};
        }
        sort(sorted.begin(), sorted.end());

        auto sq = [](double v) -> double {return v * v;};

        auto d = DBL_MAX;
        int a = -1, b = -1;
        auto update = [&](auto p1, auto p2) {
            auto dist = sq(p1.first.first - p2.first.first) + sq(p1.first.second - p2.first.second);
            if (d > dist) {
                d = dist;
                a = p1.second;
                b = p2.second; 
            }
        };

        auto cmp = [](auto p1, auto p2) {return p1.first.second < p2.first.second;};
        multiset<pair<pair<double, double>, int>, decltype(cmp)> ms(cmp);
        vector<decltype(ms)::const_iterator> its(n);
        for (int i = 0, j = 0; i < n; i++) {
            for (; j < i && sq(sorted[j].first.first - sorted[i].first.first) >= d; j++) ms.erase(its[j]);

            auto it = ms.upper_bound(sorted[i]);
            if (it != ms.begin()) {
                update(*prev(it), sorted[i]);
                for (auto p = prev(it); p != ms.begin() && sq(sorted[i].first.second - p->first.second) < d;) update(*--p, sorted[i]);
            }
            for (; it != ms.end() && sq(it->first.second - sorted[i].first.second) < d; it++) update(*it, sorted[i]);
            its[i] = ms.emplace_hint(ms.upper_bound(sorted[i]), sorted[i]);
        }

        cout << fixed << setprecision(2) << points[a].first << " " << points[a].second << " " << points[b].first << " " << points[b].second << "\n";
    }
}
