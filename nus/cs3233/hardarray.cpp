#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<long long, long long>> points(n + 1, {0, 0});
    for (int i = 0; i < n; i++) {
        cin >> points[i].first;

        if (i) points[i].first += points[i - 1].first;
    }

    for (int i = 0; i < n; i++) {
        cin >> points[i].second;

        if (i) points[i].second += points[i - 1].second;
    }
    sort(points.begin(), points.end());

    auto sq = [](long long v) -> __int128 {return (__int128) v * v;};

    __int128 d = 1e27;
    auto update = [&](auto p1, auto p2) {
        d = min(d, sq(p1.first - p2.first) + sq(p1.second - p2.second));
    };

    auto cmp = [](auto p1, auto p2) {return p1.second < p2.second;};
    multiset<pair<long long, long long>, decltype(cmp)> ms(cmp);
    vector<decltype(ms)::const_iterator> its(n + 1);
    for (int i = 0, j = 0; i <= n; i++) {
        for (; j < i && sq(points[j].first - points[i].first) >= d; j++) ms.erase(its[j]);

        auto it = ms.upper_bound(points[i]);
        if (it != ms.begin()) {
            update(*prev(it), points[i]);
            for (auto p = prev(it); p != ms.begin() && sq(points[i].second - p->second) < d;) update(*--p, points[i]);
        }
        for (; it != ms.end() && sq(it->second - points[i].second) < d; it++) update(*it, points[i]);
        its[i] = ms.emplace_hint(ms.upper_bound(points[i]), points[i]);
    }

    cout << (long long) d;
}
