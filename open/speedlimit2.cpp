#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, c;
    cin >> n >> c;

    vector<array<int, 3>> edges(n - 1);
    vector<int> limits(n - 1);
    for (int i = 0; i < n - 1; i++) {
        auto &[u, v, s] = edges[i];
        cin >> u >> v >> s;
        u--;
        v--;

        limits[i] = s;
    }
    sort(limits.begin(), limits.end());
    limits.erase(unique(limits.begin(), limits.end()), limits.end());

    vector<vector<pair<int, int>>> adj_list(n);
    for (auto [u, v, s] : edges) {
        int w = lower_bound(limits.begin(), limits.end(), s) - limits.begin();
        adj_list[u].emplace_back(v, w);
        adj_list[v].emplace_back(u, w);
    }

    auto normalize = [&](auto &intervals) {
        vector<array<long long, 4>> temp;
        for (auto [l, r, a, b] : intervals)
            if (!temp.empty() && temp.back()[2] == a && temp.back()[3] == b) temp.back()[1] = r;
            else temp.push_back({l, r, a, b});
        intervals = temp;
    };

    auto cost = [&](const auto &intervals) {
        if (intervals.empty()) return 0LL;
        auto c = LLONG_MAX;
        for (auto &[l, r, a, b] : intervals) c = min(c, a + b * limits[l]);
        return c;
    };

    vector<array<long long, 4>> temp;
    auto dfs = [&](auto &&self, int v = 0, int prev = -1, int w1 = 0) -> vector<array<long long, 4>> {
        auto sum = (long long) adj_list[v].size() * c;
        vector<array<long long, 4>> intervals;
        for (auto [u, w2] : adj_list[v])
            if (u != prev) {
                auto subintervals = self(self, u, v, w2);
                if (intervals.empty()) intervals = subintervals;
                else {
                    temp.clear();
                    for (int i = 0, j = 0; i < intervals.size() && j < subintervals.size();) {
                        auto [l1, r1, a1, b1] = intervals[i];
                        auto [l2, r2, a2, b2] = subintervals[j];
                        auto l = max(l1, l2), r = min(r1, r2);
                        if (l < r) temp.push_back({l, r, a1 + a2, b1 + b2});
                        if (r1 <= r2) i++;
                        if (r2 <= r1) j++;
                    }
                    intervals = temp;
                    normalize(intervals);
                }
                sum += cost(subintervals);
            }

        if (intervals.empty()) {
            if (!v) return {};
            return {{w1, (long long) limits.size(), -limits[w1], 1}};
        }

        if (w1 < intervals[0][0]) intervals.insert(intervals.begin(), {w1, intervals[0][0], sum, 0});
        else if (w1 > intervals[0][0]) {
            temp.clear();
            for (auto [l, r, a, b] : intervals)
                if (w1 < r) temp.push_back({max(l, (long long) w1), r, a, b});
            intervals = temp;
        }

        temp.clear();
        for (auto [l, r, a, b] : intervals) {
            int m = partition_point(limits.begin() + l, limits.begin() + r, [&](int limit) { return a + b * limit < sum; }) - limits.begin();
            if (l < m) temp.push_back({l, m, a, b});
            if (m < r) temp.push_back({m, r, sum, 0});
        }
        intervals = temp;
        if (!v) return intervals;

        for (auto &[l, r, a, b] : intervals) {
            a -= limits[w1];
            b++;
        }
        normalize(intervals);
        return intervals;
    };
    auto intervals = dfs(dfs);
    cout << cost(intervals);
}
