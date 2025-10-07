#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int C;
    cin >> C;

    while (C--) {
        int w, h, nb, nt;
        cin >> w >> h >> nb >> nt;

        vector<pair<int, int>> bottom(nb), top(nt);
        for (auto &[x, y] : bottom) cin >> x >> y;
        for (auto &[x, y] : top) cin >> x >> y;

        int k, L, R;
        cin >> k >> L >> R;

        auto speed = [&](double x, double y) {
            return k <= y ? 1e9 : x * sqrt(5 / (k - y));
        };
        double l = L, r = R;
        for (auto [x, y] : bottom) l = max(l, speed(x, y));

        for (int i = 1; i < nt; i++) {
            auto [x1, y1] = top[i - 1];
            auto [x2, y2] = top[i];

            double m = (double) (y2 - y1) / (x2 - x1), c = y1 - m * x1, x = !m ? 1e9 : -2 * (c - k) / m;
            if (x1 <= x && x <= x2) r = min(r, speed(x, m * x + c));
            else r = min({r, speed(x1, y1), speed(x2, y2)});
        }
        if (l > r) cout << "0\n";
        else cout << fixed << setprecision(5) << (r - l) / (R - L) << "\n";
    }
}
