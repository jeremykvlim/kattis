#include <bits/stdc++.h>
using namespace std;

double cross(pair<int, int> i, pair<int, int> j, pair<int, int> k) {
    return static_cast<double>(k.first - i.first) * (j.second - i.second) - static_cast<double>(k.second - i.second) * (j.first - i.first);
}

int monotone(int n, vector<pair<int, int>> &points, vector<pair<int, int>> &convex) {
    int size = 0;
    sort(points.begin(), points.end());
    points.erase(unique(points.begin(), points.end()), points.end());

    for (int i = 0; i < n; i++) {
        while (size > 1 && cross(convex[size - 2], convex[size - 1], points[i]) <= 0) size--;
        convex[size++] = points[i];
    }

    int k = size;
    for (int i = n - 2; i >= 0; i--) {
        while (size > k && cross(convex[size - 2], convex[size - 1], points[i]) <= 0) size--;
        convex[size++] = points[i];
    }

    return n > 1 ? size - 1 : size;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<int, int>> points(n), convex(n);
    for (auto &[x, y] : points) cin >> x >> y;

    int size = monotone(n, points, convex);
    double area = 0;
    if (size > 2)
        for (int i = 0; i < size; i++)
            for (int j = i + 1, k = j + 1; j < size; j++) {
                while (k % size != i && cross(convex[i], convex[j], convex[(k + 1) % size]) >= cross(convex[i], convex[j], convex[k])) ++k %= size;
                area = max(area, cross(convex[i], convex[j], convex[k % size]));
            }

    cout << fixed << setprecision(5) << area / 2.0;
}
