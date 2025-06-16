#include <bits/stdc++.h>
using namespace std;

double height(double R, double w) {
    if (w >= 1) return 2 * R;

    double l = 0, r = 2 * R, m;
    while (l + 1e-4 < r && l + l * 1e-4 < r) {
        m = l + (r - l) / 2;

        if (M_PI * m * m * (R - m / 3.0) >= w * 4 * M_PI * pow(R, 3) / 3.0) r = m;
        else l = m;
    }

    return m;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n;
        double W, L, D, V;
        cin >> n >> W >> L >> D >> V;

        vector<pair<double, double>> balls(n);
        for (auto &[R, w] : balls) cin >> R >> w;

        double l = 0, r = D, m;
        while (l + 1e-4 < r && l + l * 1e-4 < r) {
            m = l + (r - l) / 2;

            auto volume = [&]() {
                double total = 0;
                for (auto [R, w] : balls) {
                    auto h = height(R, w);
                    total += M_PI * pow(min(m, h), 2) * (R - min(m, h) / 3.0);
                }

                return total;
            };

            if (W * L * m - volume() > V) r = m;
            else l = m;
        }

        cout << fixed << setprecision(4) << m << "\n";
    }
}
