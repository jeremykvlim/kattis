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
    for (int i = 0; i <= max_x; i++) {
        vector<pair<double, double>> Y;
        for (int j = 0; j < coords.size(); j++) {
            auto [x1, y1] = coords[j];
            auto [x2, y2] = coords[(j + 1) % coords.size()];
            if (x1 > x2) {
                swap(x1, x2);
                swap(y1, y2);
            }

            auto y_coord_at_X3 = [&](double X1, double Y1, double X2, double Y2, double X3) -> double {
                return Y1 - (X1 - X3) * (Y2 - Y1) / (X2 - X1);
            };

            if (x1 <= i && x2 >= i + 1) Y.emplace_back(y_coord_at_X3(x1, y1, x2, y2, i), y_coord_at_X3(x1, y1, x2, y2, i + 1));
        }
        sort(Y.begin(), Y.end());

        for (int j = 0; j < Y.size(); j += 2) {
            auto [l1, r1] = Y[j];
            auto [l2, r2] = Y[j + 1];

            int L = ceil(max(l1, r1) - 1e-9), R = floor(min(l2, r2) + 1e-9);
            if (L < R) count += R - L;
        }
    }
    cout << count;
}
