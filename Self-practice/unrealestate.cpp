#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<double> y(2 * n);
    vector<tuple<double, double, double, double>> plots(n);
    for (int i = 0; i < n; i++) {
        cin >> get<0>(plots[i]) >> get<1>(plots[i]) >> get<2>(plots[i]) >> get<3>(plots[i]);
        y[2 * i] = get<1>(plots[i]);
        y[2 * i + 1] = get<3>(plots[i]);
    }
    sort(y.begin(), y.end());
    y.resize(unique(y.begin(), y.end()) - y.begin());
    sort(plots.begin(), plots.end());

    vector<double> x(y.size(), INT_MIN);
    double area = 0;
    for (auto &t : plots)
        for (int i = 0; i < y.size(); i++)
            if (get<1>(t) <= y[i] && y[i] < get<3>(t)) {
                x[i] = max(x[i], get<0>(t));
                if (x[i] < get<2>(t)) {
                    area += (y[i + 1] - y[i]) * (get<2>(t) - x[i]);
                    x[i] = get<2>(t);
                }
            }

    cout << fixed << setprecision(2) << area;
}
