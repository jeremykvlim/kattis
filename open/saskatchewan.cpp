#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<pair<int, int>> coords;
    int x, y, max_x = -1;
    while (cin >> x >> y) {
        coords.emplace_back(x, y);
        max_x = max(max_x, x);
    }

    auto count = 0LL;
    int n = coords.size();
    for (int x = 0; x <= max_x; x++) {
        vector<pair<double, double>> ys;
        for (int i = 0; i < n; i++) {
            auto [x1, y1] = coords[i];
            auto [x2, y2] = coords[(i + 1) % n];
            if (x1 > x2) {
                swap(x1, x2);
                swap(y1, y2);
            }

            auto y_coord_at_x3 = [&](double x3) -> double {
                return y1 - (x1 - x3) * (y2 - y1) / (x2 - x1);
            };

            if (x1 <= x && x2 >= x + 1) ys.emplace_back(y_coord_at_x3(x), y_coord_at_x3(x + 1));
        }
        sort(ys.begin(), ys.end());

        for (int i = 0; i < ys.size(); i += 2) {
            auto [l1, r1] = ys[i];
            auto [l2, r2] = ys[i + 1];

            int l = ceil(max(l1, r1) - 1e-9), r = floor(min(l2, r2) + 1e-9);
            if (l < r) count += r - l;
        }
    }
    cout << count;
}
