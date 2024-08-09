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
    auto theta = 2 * M_PI / sectors, a = 0.;
    for (int i = 0; i < sectors; i++) {
        auto angle = [&](auto theta) {
            auto xp = R * cos(theta), yp = R * sin(theta);

            vector<double> angles;
            for (int i = 0; i < n; i++) angles.emplace_back(atan2(y[i] - yp, x[i] - xp));
            sort(angles.begin(), angles.end());

            auto diff = [](auto a1, auto a2) {
                while (a2 - a1 < -1e-11) a2 += 2 * M_PI;
                return a2 - a1;
            };

            double gap = 0;
            for (int i = 0; i < angles.size(); i++) gap = max(gap, diff(angles[i], angles[(i + 1) % angles.size()]));

            return 2 * M_PI - gap;
        };

        double l = i * theta, r = i * theta + theta, mid1, mid2;
        while (l + 1e-9 < r && l + l * 1e-9 < r) {
            mid1 = l + (r - l) / 3, mid2 = r - (r - l) / 3;

            if (angle(mid1) > angle(mid2)) r = mid2;
            else l = mid1;
        }

        a = max(a, angle(l));
    }

    cout << fixed << setprecision(6) << a;
}
