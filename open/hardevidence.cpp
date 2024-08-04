#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, R;
    cin >> n >> R;

    vector<double> x(n), y(n);
    for (int i = 0; i < n; i++) cin >> x[i] >> y[i];

    int sectors = 9;
    auto theta = 2 * M_PI / sectors, a = -1e9;
    for (int i = 0; i < sectors; i++) {
        double l = i * theta, r = i * theta + theta, mid1, mid2;

        auto angle = [&](auto ang) {
            auto xp = R * cos(ang), yp = R * sin(ang);

            vector<double> angles;
            for (int i = 0; i < n; i++) angles.emplace_back(atan2(y[i] - yp, x[i] - xp));
            sort(angles.begin(), angles.end());

            auto diff = [](auto a, auto b) {
                while (a - b < -1e-11) a += 2 * M_PI;
                return a - b;
            };

            double gap = 0;
            for (int i = 1; i < angles.size(); i++) gap = max(gap, diff(angles[i], angles[i - 1]));
            gap = max(gap, diff(angles[0], angles.back()));

            return 2 * M_PI - gap;
        };

        while (l + 1e-9 < r && l + l * 1e-9 < r) {
            mid1 = (l * 2 + r) / 3, mid2 = (l + r * 2) / 3;

            if (angle(mid1) > angle(mid2)) r = mid2;
            else l = mid1;
        }

        a = max(a, angle(l));
    }

    cout << fixed << setprecision(10) << a;
}
