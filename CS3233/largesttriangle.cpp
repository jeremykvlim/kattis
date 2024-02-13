#include <bits/stdc++.h>
using namespace std;

double cross(pair<int, int> i, pair<int, int> j, pair<int, int> k) {
    return static_cast<double>(k.first - i.first) * (j.second - i.second) - static_cast<double>(k.second - i.second) * (j.first - i.first);
}

int monotone(vector<pair<int, int>> &points, vector<pair<int, int>> &convex) {
    sort(points.begin(), points.end());
    points.erase(unique(points.begin(), points.end()), points.end());

    for (auto p : points) {
        while (convex.size() > 1 && cross(convex[convex.size() - 2], convex[convex.size() - 1], p) <= 0) convex.pop_back();
        convex.emplace_back(p);
    }

    int s = convex.size();
    points.pop_back();
    reverse(points.begin(), points.end());
    for (auto p : points) {
        while (convex.size() > s && cross(convex[convex.size() - 2], convex[convex.size() - 1], p) <= 0) convex.pop_back();
        convex.emplace_back(p);
    }

    return convex.size();
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<int, int>> points(n), convex;
    for (auto &[x, y] : points) cin >> x >> y;

    int size = monotone(points, convex);
    double area = 0;
    if (size > 2)
        for (int i = 0; i < size; i++)
            for (int j = i + 1, k = j + 1; j < size; j++) {
                while (k % size != i && cross(convex[i], convex[j], convex[(k + 1) % size]) >= cross(convex[i], convex[j], convex[k])) ++k %= size;
                area = max(area, cross(convex[i], convex[j], convex[k % size]));
            }

    cout << fixed << setprecision(5) << area / 2;
}
