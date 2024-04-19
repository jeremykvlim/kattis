#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<double, double>> coords(n);
    for (auto &[x, y] : coords) cin >> x >> y;
    sort(coords.begin(), coords.end());

    auto dist = DBL_MAX;
    for (int i = 1; i < n; i++) {
        auto [x1, y1] = coords[i - 1];
        auto [x2, y2] = coords[i];
        if (x1 != x2 && y1 * x2 - y2 * x1 > 0) dist = min(dist, (y1 * x2 * x2 - y2 * x1 * x1) / (y1 * x2 - y2 * x1));
    }

    cout << fixed << setprecision(6) << dist;
}
