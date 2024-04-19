#include <bits/stdc++.h>
using namespace std;

constexpr double EPSILON = 1e-4;

double height(double radius, double weight) {
    if (1.0 <= weight) return 2.0 * radius;
    double l = 0, r = 2.0 * radius, m;
    while (l + EPSILON < r) {
        m = l + (r - l) / 2;
        if (M_PI * m * m * (radius - m / 3.0) >= weight * 4 * M_PI * pow(radius, 3) / 3.0) r = m;
        else l = m;
    }

    return m;
}

double volume(double level, vector<pair<double, double>> &balls) {
    double total = 0;
    for (auto [r, w] : balls) {
        auto h = height(r, w);
        total += M_PI * pow(min(level, h), 2) * (r - min(level, h) / 3.0);
    }
    return total;
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
        for (auto &[r, w] : balls) cin >> r >> w;
        double l = 0, r = D, m;
        while (l + EPSILON < r) {
            m = l + (r - l) / 2;
            if (W * L * m - volume(m, balls) > V) r = m;
            else l = m;
        }

        cout << fixed << setprecision(4) << m << "\n";
    }
}
