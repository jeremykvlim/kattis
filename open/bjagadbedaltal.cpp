#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    double sum = 0, sum_log = 0;
    vector<double> x(n);
    for (auto &xi : x) {
        cin >> xi;

        sum += xi;
        sum_log += log(xi);
    }

    nth_element(x.begin(), x.begin() + n / 2, x.end());
    auto a_mean = sum / n, g_mean = exp(sum_log / n), median = x[n / 2];
    for (;;) {
        auto s = a_mean + g_mean + median, a = s / 3, g = cbrt(a_mean * g_mean * median), m = s - min(a_mean, min(g_mean, median)) - max(a_mean, max(g_mean, median));
        if (max({fabs(a_mean - a), fabs(g_mean - g), fabs(median - m)}) < 1e-5) break;
        tie(a_mean, g_mean, median) = tie(a, g, m);
    }
    cout << fixed << setprecision(5) << a_mean;
}
