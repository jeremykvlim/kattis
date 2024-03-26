#include <bits/stdc++.h>
using namespace std;

double cross(pair<long long, long long> i, pair<long long, long long> j, pair<long long, long long> k) {
    return (double) (k.first - i.first) * (double) (j.second - i.second) - (double) (k.second - i.second) * (double) (j.first - i.first);
}

void monotone(vector<pair<long long, long long>> &points, deque<pair<long long, long long>> &convex) {
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

    vector<pair<long long, long long>> points;
    while (n--) {
        long long x, y;
        char c;
        cin >> x >> y >> c;

        if (c == 'Y') points.emplace_back(x, y);
    }

    deque<pair<long long, long long>> convex;
    monotone(points, convex);
    
    convex.pop_front();
    reverse(convex.begin(), convex.end());
    cout << convex.size() << "\n";
    for (auto [x, y] : convex) cout << x << " " << y << "\n";
}
