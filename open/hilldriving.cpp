#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        double a, b, vmax, f;
        int R;
        cin >> a >> b >> vmax >> f >> R;

        vector<double> dist(R), slope(R);
        for (int i = 0; i < R; i++) {
            double x, y;
            cin >> x >> y;

            dist[i] = hypot(x / 1e3, y / 1e3);
            slope[i] = y / x;
        }

        double l = 0, r = vmax, mid;
        while (l + 1e-9 < r && l + l * 1e-9 < r) {
            mid = l + (r - l) / 2;

            double fuel = 0;
            for (int i = 0; i < R; i++) fuel += max(0., a * mid + b * slope[i]) * dist[i];

            if (fuel <= f) l = mid;
            else r = mid;
        }

        if (l == 0) cout << "IMPOSSIBLE\n";
        else {
            double time = 0;
            for (int i = 0; i < R; i++) time += dist[i] / min(vmax, max(-slope[i] * b / a, l));
            cout << fixed << setprecision(6) << time << "\n";
        }
    }
}
