#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long h0;
    cin >> n >> h0;

    auto hl = h0, hr = h0;
    vector<pair<long long, long long>> requests(n);
    for (auto &[u, v] : requests) {
        cin >> u >> v;

        hl = min({hl, u, v});
        hr = max({hr, u, v});
    }

    auto solve = [&](int sgn) {
        vector<pair<long long, long long>> intervals;
        for (auto [u, v] : requests) {
            u *= sgn;
            v *= sgn;
            if (u > v) intervals.emplace_back(v, u);
        }
        sort(intervals.begin(), intervals.end(), [&](auto p1, auto p2) { return p1.second < p2.second; });

        auto [l, r] = minmax({sgn * hl, sgn * hr});
        auto h = sgn * h0, base = h + r - 2 * l;
        int m = intervals.size();
        vector<long long> suff(m + 1, r);
        for (int i = m - 1; ~i; i--) suff[i] = min(suff[i + 1], intervals[i].first);

        auto dist = LLONG_MAX;
        for (int i = 0; i <= m; i++) dist = min(dist, base + r - suff[i] + (!i ? 0 : 2 * max(0LL, intervals[i - 1].second - h)));

        sort(intervals.begin(), intervals.end());
        vector<pair<long long, long long>> temp;
        for (auto [a, b] : intervals)
            if (!temp.empty() && a < temp.back().second) temp.back().second = max(temp.back().second, b);
            else temp.emplace_back(a, b);
        intervals = temp;

        m = intervals.size();
        long long pref = 0, cost = 0;
        for (int i = 0; i <= m; i++) {
            if (i == m || intervals[i].first >= h) dist = min(dist, base + 2 * pref + cost + (i == m ? 0 : r - intervals[i].first));
            if (i < m) {
                pref += intervals[i].second - intervals[i].first;
                cost = min(cost, 2 * max(0LL, intervals[i].second - h) - 2 * pref);
            }
        }
        return dist;
    };
    cout << min(solve(1), solve(-1));
}
