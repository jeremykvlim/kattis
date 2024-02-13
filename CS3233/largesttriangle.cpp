#include <bits/stdc++.h>
using namespace std;

double cross(pair<int, int> i, pair<int, int> j, pair<int, int> k) {
    return (double) (k.first - i.first) * (double) (j.second - i.second) - (double) (k.second - i.second) * (double) (j.first - i.first);
}

void monotone(vector<pair<int, int>> &points, deque<pair<int, int>> &convex) {
    sort(points.begin(), points.end());
    points.erase(unique(points.begin(), points.end()), points.end());

    for (auto p : points) {
        while (convex.size() > 1 && cross(convex[1], convex[0], p) <= 0) convex.pop_front();
        convex.emplace_front(p);
    }

    int s = convex.size();
    points.pop_back();
    reverse(points.begin(), points.end());
    for (auto p : points) {
        while (convex.size() > s && cross(convex[1], convex[0], p) <= 0) convex.pop_front();
        convex.emplace_front(p);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<int, int>> points(n);
    for (auto &[x, y] : points) cin >> x >> y;

    deque<pair<int, int>> convex;
    monotone(points, convex);
    reverse(convex.begin(), convex.end());
    int s = convex.size();
    double area = 0;
    if (convex.size() > 2)
        for (int i = 0; i < s; i++)
            for (int j = i + 1, k = j + 1; j < s; j++) {
                while (k % s != i && cross(convex[i], convex[j], convex[(k + 1) % s]) >= cross(convex[i], convex[j], convex[k])) ++k %= s;
                area = max(area, cross(convex[i], convex[j], convex[k % s]));
            }

    cout << fixed << setprecision(5) << area / 2;
}
