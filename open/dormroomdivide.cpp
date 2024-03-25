#include <bits/stdc++.h>
using namespace std;

double area(pair<double, double> p1, pair<double, double> p2, pair<double, double> p3) {
    return 0.5 * fabs(p1.first * p2.second + p2.first * p3.second + p3.first * p1.second
                    - p2.first * p1.second - p3.first * p2.second - p1.first * p3.second);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<double, double>> coords(n);
    for (auto &[x, y] : coords) cin >> x >> y;

    vector<double> pref(n - 1, 0);
    double sum = 0;
    for (int i = 1; i < n - 1; i++) {
        sum += area(coords[0], coords[i], coords[i + 1]);
        pref[i] = sum;
    }
  
    int j;
    for (int i = 0; i < n - 1; i++)
        if (sum <= 2 * pref[i]) {
            j = i;
            break;
        }

    auto calc = [&](int i) -> tuple<double, double, double> {
        return make_tuple(coords[i].first - coords[0].first,
                          coords[0].second - coords[i].second,
                          sum - 2 * pref[i - 1] + coords[i].first * coords[0].second - coords[0].first * coords[i].second);
    };

    auto [x1, y1, a1] = calc(j);
    auto [x2, y2, a2] = calc(j + 1);
    auto x = (x1 * a2 - x2 * a1) / (x1 * y2 - x2 * y1), y = (a1 - y1 * x) / x1;

    cout << fixed << setprecision(6) << x << " " << y;
}
