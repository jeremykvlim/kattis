#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    int k;
    cin >> n >> k;

    vector<array<double, 3>> fruits(k);
    for (auto &[b, c, d] : fruits) cin >> b >> c >> d;

    auto calc_x = [&](double b, double c, double d, double m) -> long long {
        if (!c) return 0;
        if (!d) return c - b >= m;
        if (!b && !m) return n;

        long long x = floor((-1 + sqrt(1 + 4 * (c * d / (b + m)))) / 2);
        if (x < d) return 0;
        if (x >= d + n - 1) return n;
        return x - d + 1;
    };

    double l = 0, r = 1e9, m;
    while (l + 1e-15 < r && l + l * 1e-15 < r) {
        m = l + (r - l) / 2;

        auto plants = 0LL;
        for (auto [b, c, d] : fruits) plants += calc_x(b, c, d, m);

        if (plants >= n) l = m;
        else r = m;
    }

    auto plants = 0LL;
    vector<long long> x(k, 0);
    auto profit = 0.;
    for (int i = 0; i < k; i++) {
        auto [b, c, d] = fruits[i];
        x[i] = calc_x(b, c, d, l);
        if (!x[i]) continue;
        plants += x[i];
        profit += x[i] * c / (d + x[i]) - x[i] * b;
    }

    if (plants > n) {
        auto delta = [&](double b, double c, double d, long long x) -> double {
            if (!d) return x == 1 ? c - b : -b;
            return c * d / ((d + x - 1) * (d + x)) - b;
        };
        
        priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pq;
        for (int i = 0; i < k; i++)
            if (x[i]) {
                auto [b, c, d] = fruits[i];
                pq.emplace(delta(b, c, d, x[i]), i);
            }

        while (plants-- > n) {
            auto [g, i] = pq.top();
            pq.pop();

            profit -= g;
            if (--x[i]) {
                auto [b, c, d] = fruits[i];
                pq.emplace(delta(b, c, d, x[i]), i);
            }
        }
    }
    cout << fixed << setprecision(8) << profit;
}
