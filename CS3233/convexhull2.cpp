#include <bits/stdc++.h>
using namespace std;

double cross(pair<long long, long long> i, pair<long long, long long> j, pair<long long, long long> k) {
    return (double) (k.first - i.first) * (double) (j.second - i.second) - (double) (k.second - i.second) * (double) (j.first - i.first);
}

void monotone(vector<pair<long long, long long>> &points, vector<pair<long long, long long>> &convex) {
    sort(points.begin(), points.end());
    points.erase(unique(points.begin(), points.end()), points.end());

    for (auto p : points) {
        while (convex.size() > 1 && cross(convex[convex.size() - 2], convex.back(), p) > 0) convex.pop_back();
        convex.emplace_back(p);
    }

    int s = convex.size();
    points.pop_back();
    reverse(points.begin(), points.end());
    for (auto p : points) {
        while (convex.size() > s && cross(convex[convex.size() - 2], convex.back(), p) > 0) convex.pop_back();
        convex.emplace_back(p);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<long long, long long>> points, convex;
    while (n--) {
        long long x, y;
        char c;
        cin >> x >> y >> c;

        if (c == 'Y') points.emplace_back(x, y);
    }

    monotone(points, convex);
    reverse(convex.begin(), convex.end());
    convex.pop_back();
    cout << convex.size() << "\n" << convex.front().first << " " << convex.front().second << "\n";
    reverse(convex.begin(), convex.end());
    convex.pop_back();
    for (auto [x, y] : convex) cout << x << " " << y << "\n";
}
