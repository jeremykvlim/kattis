#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n && n) {
        vector<pair<double, double>> points(n);
        vector<tuple<double, double, int>> sorted(n);
        for (int i = 0; i < n; i++) {
            cin >> points[i].first >> points[i].second;

            sorted[i] = {points[i].first, points[i].second, i};
        }
        sort(sorted.begin(), sorted.end());

        auto sq = [](double v) -> double {return v * v;};

        auto d = DBL_MAX;
        int p1 = -1, p2 = -1;
        auto update = [&](auto t1, auto t2) {
            auto dist = sq(get<0>(t1) - get<0>(t2)) + sq(get<1>(t1) - get<1>(t2));
            if (d > dist) {
                d = dist;
                p1 = get<2>(t1);
                p2 = get<2>(t2); }
        };

        auto cmp = [](auto t1, auto t2) {return get<1>(t1) < get<1>(t2);};
        multiset<tuple<double, double, int>, decltype(cmp)> ms(cmp);
        vector<decltype(ms)::const_iterator> its(n);
        for (int i = 0, j = 0; i < n; ++i) {
            while (j < i && (sq(get<0>(sorted[j]) - get<0>(sorted[i])) >= d)) ms.erase(its[j++]);

            auto it = ms.upper_bound(sorted[i]);
            if (it != ms.begin()) {
                update(*prev(it), sorted[i]);
                for (auto p = prev(it); p != ms.begin() && sq(get<1>(sorted[i]) - get<1>(*p)) < d;) update(*--p, sorted[i]);
            }
            for (; it != ms.end() && sq(get<1>(*it) - get<1>(sorted[i])) < d; it++) update(*it, sorted[i]);
            its[i] = ms.emplace_hint(ms.upper_bound(sorted[i]), sorted[i]);
        }

        cout << fixed << setprecision(2) << points[p1].first << " " << points[p1].second << " " << points[p2].first << " " << points[p2].second << "\n";
    }
}
