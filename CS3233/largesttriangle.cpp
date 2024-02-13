#include <bits/stdc++.h>
using namespace std;

double cross(pair<int, int> i, pair<int, int> j, pair<int, int> k) {
    return (double) (k.first - i.first) * (double) (j.second - i.second) - (double) (k.second - i.second) * (double) (j.first - i.first);
}

void monotone(vector<pair<int, int>> &points, vector<pair<int, int>> &convex) {
    sort(points.begin(), points.end());
    points.erase(unique(points.begin(), points.end()), points.end());

    for (auto p : points) {
        while (convex.size() > 1 && cross(convex[convex.size() - 2], convex.back(), p) <= 0) convex.pop_back();
        convex.emplace_back(p);
    }

    int s = convex.size();
    points.pop_back();
    reverse(points.begin(), points.end());
    for (auto p : points) {
        while (convex.size() > s && cross(convex[convex.size() - 2], convex.back(), p) <= 0) convex.pop_back();
        convex.emplace_back(p);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<int, int>> points(n), convex;
    for (auto &[x, y] : points) cin >> x >> y;

    monotone(points, convex);
    double area = 0;
    if (convex.size() > 2)
        for (int i = 0; i < convex.size(); i++)
            for (int j = i + 1, k = j + 1; j < convex.size(); j++) {
                while (k % convex.size() != i && cross(convex[i], convex[j], convex[(k + 1) % convex.size()]) >= cross(convex[i], convex[j], convex[k])) ++k %= convex.size();
                area = max(area, cross(convex[i], convex[j], convex[k % convex.size()]));
            }

    cout << fixed << setprecision(5) << area / 2;
}
