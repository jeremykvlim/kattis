#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<double> y;
    vector<tuple<double, double, double, double>> plots(n);
    for (auto &[x1, y1, x2, y2] : plots) {
        cin >> x1 >> y1 >> x2 >> y2;

        y.emplace_back(y1);
        y.emplace_back(y2);
    }
    sort(y.begin(), y.end());
    y.resize(unique(y.begin(), y.end()) - y.begin());
    sort(plots.begin(), plots.end());

    vector<double> x(y.size(), INT_MIN);
    double area = 0;
    for (auto [x1, y1, x2, y2] : plots)
        for (int i = 0; i < y.size(); i++)
            if (y1 <= y[i] && y[i] < y2) {
                x[i] = max(x[i], x1);
                if (x[i] < x2) {
                    area += (y[i + 1] - y[i]) * (x2 - x[i]);
                    x[i] = x2;
                }
            }

    cout << fixed << setprecision(2) << area;
}
