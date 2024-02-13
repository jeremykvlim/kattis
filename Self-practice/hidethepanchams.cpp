#include <bits/stdc++.h>
using namespace std;

double cross(pair<int, int> i, pair<int, int> j, pair<int, int> k) {
    return (double) (k.first - i.first) * (double) (j.second - i.second) - (double) (k.second - i.second) * (double) (j.first - i.first);
}

void monotone(vector<pair<int, int>> &points, deque<pair<int, int>> &convex) {
    sort(points.begin(), points.end());
    points.erase(unique(points.begin(), points.end()), points.end());

    for (auto p : points) {
        while (convex.size() > 1 && cross(convex[1], convex[0], p) > 0) convex.pop_front();
        convex.emplace_front(p);
    }

    int s = convex.size();
    points.pop_back();
    reverse(points.begin(), points.end());
    for (auto p : points) {
        while (convex.size() > s && cross(convex[1], convex[0], p) > 0) convex.pop_front();
        convex.emplace_front(p);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<int, int>> houses(n);
    for (auto &[x, y]: houses) cin >> x >> y;

    vector<double> dist(1 << n, 0), dp(1 << n, LLONG_MAX);
    dp[0] = 0;
    for (int i = 0; i < 1 << n; i++) {
        vector<pair<int, int>> sub;
        for (int j = 0; j < n; j++)
            if (i & 1 << j) sub.emplace_back(houses[j]);

        if (sub.empty()) continue;
        deque<pair<int, int>> convex;
        monotone(sub, convex);
        for (int j = convex.size() - 1, k = 0; k < convex.size(); j = k++)
            dist[i] += hypot(convex[j].first - convex[k].first, convex[j].second - convex[k].second);
    }

    for (int i = 1; i < 1 << n; i++)
        for (int j = i; j; j = (j - 1) & i)
            if (__builtin_popcount(j) >= 3) dp[i] = min(dp[i], dp[i ^ j] + dist[j]);

    cout << fixed << setprecision(6) << dp.back();
}
